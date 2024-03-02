#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int calc_grade(string text);
int _calc_index(int letters, int words, int sentences);

int main(void)
{
    string text = get_string("Text: ");

    int grade = calc_grade(text);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int calc_grade(string text)
{
    int letters = 0, words = 1, sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (isalpha(c))
        {
            letters++;
            continue;
        }
        if (isblank(c))
        {
            words++;
            continue;
        }
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }
    int grade = _calc_index(letters, words, sentences);
    return grade;
}

int _calc_index(int letters, int words, int sentences)
{
    float L = letters / (float) words * 100;
    float S = sentences / (float) words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    return round(index);
}
