// Julia Enriquetto de Brito
// libraries
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }

    string key = argv[1]; // get what user put as an argument

    // check if the key contain only alphabetic characters
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
    }

    // check the length
    if (strlen(key) < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check if each letter is unique
    for (int i = 0; i < strlen(key); i++)
    {
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (toupper(key[i]) == toupper(key[j]))
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    // get plaintext
    string plaintext = get_string("plaintext: ");

    // converts to uppercase
    for (int i = 0; i < strlen(key); i++)
    {
        if (islower(key[i]))
        {
            key[i] = key[i] - 32;
        }
    }

    printf("ciphertext: ");

    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isupper(plaintext[i]))
        {
            int letter = plaintext[i] - 65; // 65 'A'
            printf("%c", key[letter]);
        }
        else if (islower(plaintext[i]))
        {
            int letter = plaintext[i] - 97; // 97 'a'
            printf("%c", key[letter] + 32);
        }
        else
            printf("%c", plaintext[i]); // keep non-alphabetic characters
    }
    printf("\n");

    return 0; // SUCCESS :)
}

// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// NQXPOMAFTRHLZGECYJIUWSKDVB (key)
