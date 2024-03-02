#include <cs50.h>
#include <stdio.h>

int get_start(void);
int get_end(int start);
int calculate_years(int start, int end);

int main(void)
{
    int start = get_start();

    int end = get_end(start);

    int years = calculate_years(start, end);

    // Print number of years
    printf("Years: %i\n", years);
}

// Prompt for start size
int get_start(void)
{
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);
    return start;
}

// Prompt for end size
int get_end(int start)
{
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);
    return end;
}

// Calculate number of years until we reach threshold
int calculate_years(int start, int end)
{
    int llamas = start;
    int years = 0;
    for (years = 0; llamas < end; years++)
    {
        llamas += llamas / 3 - llamas / 4;
    }
    return years;
}
