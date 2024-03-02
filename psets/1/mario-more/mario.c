#include <cs50.h>
#include <stdio.h>

int get_size(void);
void print_bricks(int size);
void _print_left(int size, int i);
void _print_right(int size, int i);

int main(void)
{
    int size = get_size();

    print_bricks(size);
}

int get_size(void)
{
    int size;
    do
    {
        size = get_int("Size: ");
    }
    while (size < 1 || size > 8);
    return size;
}

void print_bricks(int size)
{
    for (int i = 1; i <= size; i++)
    {
        _print_left(size, i);
        printf("  ");
        _print_right(size, i);
        printf("\n");
    }
}

void _print_left(int size, int i)
{
    for (int j = size; j > 0; j--)
    {
        if (j <= i)
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }
}

void _print_right(int size, int i)
{
    for (int j = 0; j < size; j++)
    {
        if (j < i)
        {
            printf("#");
        }
    }
}
