const artists = document.querySelectorAll(".artist button");
artists.forEach(button => {
    button.addEventListener("click", evt => {
        artists.forEach(btn => {
            btn.classList.remove("btn-success", "btn-danger");
            btn.classList.add("btn-secondary");
        });
        if (evt.target.innerText == "Ado") {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-success");
        } else {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-danger");
        }
    });
});
const areas = document.querySelectorAll(".area button");
areas.forEach(button => {
    button.addEventListener("click", evt => {
        areas.forEach(btn => {
            btn.classList.remove("btn-success", "btn-danger");
            btn.classList.add("btn-secondary");
        });
        if (evt.target.innerText == "Web development") {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-success");
        } else {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-danger");
        }
    });
});
const games = document.querySelectorAll(".game button");
games.forEach(button => {
    button.addEventListener("click", evt => {
        games.forEach(btn => {
            btn.classList.remove("btn-success", "btn-danger");
            btn.classList.add("btn-secondary");
        });
        if (evt.target.innerText == "Crosscode") {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-success");
        } else {
            evt.target.classList.remove("btn-secondary");
            evt.target.classList.add("btn-danger");
        }
    });
});
