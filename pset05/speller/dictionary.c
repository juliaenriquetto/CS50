#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Number of words in dictionary
int num_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);
    node *cursor = table[hash_value];
    while (cursor != NULL)
    {
        // Traversing linked list
        if (strcasecmp(cursor->word, word) == 0) // string compare
        {
            return true;
        }
        cursor = cursor->next; // move to next node
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        // Convert to uppercase and sum ASCII values
        hash_value += toupper(word[i]);
    }

    // Limit hash value to the size of the hash table
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize the hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    char buffer[LENGTH + 1]; // Temporary buffer for words

    // Add each word to the hash table
    while (fscanf(source, "%s", buffer) != EOF)
    {
        // Create space for a new node
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL) // Check if malloc was successful
        {
            return false;
        }

        int hash_value = hash(buffer);

        // Copy the word into the new node
        strcpy(new_word->word, buffer);

        // Insert the new node into the hash table
        new_word->next = table[hash_value];
        table[hash_value] = new_word;

        num_words++;
    }

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp); // Free each node
        }
    }
    return true;
}
