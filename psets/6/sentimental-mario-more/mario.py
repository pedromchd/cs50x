def main():
    height = get_height()
    draw(height, height)


def get_height():
    while True:
        try:
            height = int(input("Height: "))
            if height > 0 and height <= 8:
                return height
        except ValueError:
            continue


def draw(height, size):
    if height <= 0:
        return
    draw(height - 1, size)
    print(" " * (size - height), end="")
    print("#" * height, end="")
    print("  ", end="")
    print("#" * height)


main()
