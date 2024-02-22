// Julia Enriquetto de Brito
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// points for each letter on scrabble game
int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 8, 4, 10};

int score(string word);

int main(int argc, string argv[])
{
    // get words of each player
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    int score1 = score(word1);
    int score2 = score(word2);

    if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else
        printf("Player 2 wins!\n");

    return 0; // sucess!
}

int score(string word)
{
    int score = 0;
    // walks through each character
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isupper(word[i]))
        {
            score = score + points[word[i] - 65]; // according to ASCII table
        }
        else if (islower(word[i]))
        {
            score = score + points[word[i] - 97];
        }
    }
    return score;
}
