// Julia Enriquetto de Brito
#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    string text = get_string("Text: ");
    int letter = 0, word = 1, sentence = 0;

    for (int i = 0, n = strlen(text); i < n; i++) // percorre o texto
    {
        if (isalpha(text[i]))
        {
            letter++;
        }
        else if (isspace(text[i])) // if there's a space then we have a word
        {
            word++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?') // each '.', '!', '?' indicates a sentence
        {
            sentence++;
        }
    }

    double L = (letter / (float) word) * 100;
    double S = (sentence / (float) word) * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8); // Coleman Liau Index

    if (index < 1)
        printf("Before Grade 1\n");
    else if (index >= 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", index);

    return 0;
}
