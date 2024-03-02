def main():
    card = int(input(("Card number: ")))
    print(f"Number: {card}")

    sum1 = 0
    sum2 = 0
    holder = 0
    i = 0
    while card > 0:
        if (card > 9 and card < 100) or card == 4:
            holder = card

        trunc = float(card / 10 * 10)
        digit = int(card % trunc if trunc else card)

        if i % 2:
            digit *= 2
            sum1 += digit if digit < 10 else 1 + digit % 10
        else:
            sum2 += digit
        i += 1
        card /= 10

    total = int(sum1 + sum2)
    if total % 10:
        print("INVALID")
    else:
        match holder:
            case 4:
                print("VISA") if (i == 12 or i == 16) else print("INVALID")
            case 34 | 37:
                print("AMEX") if i == 15 else print("INVALID")
            case 51 | 52 | 53 | 54 | 55:
                print("MASTERCARD") if i == 16 else print("INVALID")
            case _:
                print("INVALID")


main()
