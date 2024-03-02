def main():
    change = get_change()
    coins = []
    get_coins(change, coins)

    numbers = 0
    for i in coins:
        numbers += i

    print(numbers)


def get_change():
    while True:
        try:
            change = float(input("Change owed: "))
            if change >= 0:
                return int(change * 100)
        except ValueError:
            continue


def get_coins(value, coins):
    if value == 0:
        return 0

    if value >= 25:
        get_coins(value % 25, coins)
        coins.append(value // 25)
        return

    if value >= 10:
        get_coins(value % 10, coins)
        coins.append(value // 10)
        return

    if value >= 5:
        get_coins(value % 5, coins)
        coins.append(value // 5)
        return

    if value >= 1:
        get_coins(value % 1, coins)
        coins.append(value // 1)
        return


main()
