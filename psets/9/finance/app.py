import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    stocks = db.execute(
        "SELECT * FROM stocks WHERE shares > 0 AND user_id = ? ORDER BY symbol ASC",
        session["user_id"],
    )

    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]

    cash["total"] = cash["cash"]

    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total"] = float(stock["price"]) * int(stock["shares"])
        cash["total"] += float(stock["total"])

    return render_template("index.html", stocks=stocks, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("missing symbol")

        if not request.form.get("shares"):
            return apology("missing shares")

        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol")

        if (
            not request.form.get("shares").isdecimal()
            or int(request.form.get("shares")) <= 0
        ):
            return apology("invalid shares")

        stock = lookup(request.form.get("symbol"))

        shares = int(request.form.get("shares"))

        price = float(stock["price"]) * shares

        cash = float(
            db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
                "cash"
            ]
        )

        if cash < price:
            return apology("can't afford")

        rows = db.execute(
            "SELECT shares FROM stocks WHERE symbol = ? AND user_id = ?",
            stock["symbol"],
            session["user_id"],
        )

        db.execute("BEGIN TRANSACTION")

        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", cash - price, session["user_id"]
        )

        db.execute(
            "INSERT INTO history (symbol, shares, price, user_id) VALUES (?, ?, ?, ?)",
            stock["symbol"],
            shares,
            stock["price"],
            session["user_id"],
        )

        if not rows:
            db.execute(
                "INSERT INTO stocks (symbol, shares, user_id) VALUES (?, ?, ?)",
                stock["symbol"],
                shares,
                session["user_id"],
            )
        else:
            shares += int(rows[0]["shares"])
            db.execute(
                "UPDATE stocks SET shares = ? WHERE symbol = ? AND user_id = ?",
                shares,
                stock["symbol"],
                session["user_id"],
            )

        db.execute("COMMIT TRANSACTION")

        flash("Bought!")

        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute(
        "SELECT * FROM history WHERE user_id = ? ORDER BY transacted DESC",
        session["user_id"],
    )

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("missing symbol")

        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol")

        stock = lookup(request.form.get("symbol"))

        return render_template("quoted.html", stock=stock)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username")

        if not request.form.get("password"):
            return apology("must provide password")

        if request.form.get("confirmation") != request.form.get("password"):
            return apology("passwords don't match")

        if db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        ):
            return apology("username is not available")

        hash = generate_password_hash(request.form.get("password"))

        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            request.form.get("username"),
            hash,
        )

        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    symbols = {
        row["symbol"]: row["shares"]
        for row in db.execute(
            "SELECT symbol, shares FROM stocks WHERE shares > 0 AND user_id = ? ORDER BY symbol ASC",
            session["user_id"],
        )
    }

    print(symbols)

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("missing symbol")

        if not request.form.get("shares"):
            return apology("missing shares")

        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol")

        if (
            not request.form.get("shares").isdecimal()
            or int(request.form.get("shares")) <= 0
        ):
            return apology("invalid shares")

        if request.form.get("symbol") not in symbols.keys():
            return apology("symbol not owned")

        if int(request.form.get("shares")) > int(symbols[request.form.get("symbol")]):
            return apology("too many shares")

        stock = lookup(request.form.get("symbol"))

        shares = int(request.form.get("shares"))

        price = float(stock["price"]) * shares

        cash = float(
            db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0][
                "cash"
            ]
        )

        db.execute("BEGIN TRANSACTION")

        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", cash + price, session["user_id"]
        )

        shares *= -1
        db.execute(
            "INSERT INTO history (symbol, shares, price, user_id) VALUES (?, ?, ?, ?)",
            stock["symbol"],
            shares,
            stock["price"],
            session["user_id"],
        )

        shares += int(symbols[stock["symbol"]])
        db.execute(
            "UPDATE stocks SET shares = ? WHERE symbol = ? AND user_id = ?",
            shares,
            stock["symbol"],
            session["user_id"],
        )

        db.execute("COMMIT TRANSACTION")

        flash("Sold!")

        return redirect("/")

    return render_template("sell.html", symbols=symbols.keys())
