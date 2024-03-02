text = input("Text: ")

letters = 0
words = 1
sentences = 0

for c in text:
    if c.upper() >= "A" and c.upper() <= "Z":
        letters += 1

    if c == " ":
        words += 1

    if c in [".", "!", "?"]:
        sentences += 1

L = letters * 100 / words
S = sentences * 100 / words

grade = int(0.0588 * L - 0.296 * S - 15.8 + 0.5)

if grade > 16:
    grade = "16+"
elif grade < 1:
    grade = "Before Grade 1..."

print(f"Grade {grade}")
