#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card = get_long("Card number: ");
    printf("Number: %li\n", card);

    int sum1 = 0, sum2 = 0, holder = 0, i = 0;
    while (card > 0)
    {
        if ((card > 9 && card < 100) || card == 4)
        {
            holder = card;
        }

        long trunc = card / 10 * 10;
        int digit = (trunc) ? card % trunc : card;

        if (i % 2)
        {
            digit *= 2;
            sum1 += (digit < 10) ? digit : 1 + digit % 10;
        }
        else
        {
            sum2 += digit;
        }
        i++, card /= 10;
    }

    int total = sum1 + sum2;
    if (total % 10)
    {
        printf("INVALID\n");
    }
    else
    {
        switch (holder)
        {
            case 4:
                (i == 13 || i == 16) ? printf("VISA\n") : printf("INVALID\n");
                break;
            case 34:
            case 37:
                (i == 15) ? printf("AMEX\n") : printf("INVALID\n");
                break;
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                (i == 16) ? printf("MASTERCARD\n") : printf("INVALID\n");
                break;
            default:
                printf("INVALID\n");
        }
    }
}
