// Declares a dictionary's functionality

// those lines just ensure that, even though dictionary.c and speller.c #include this file
// clang will only compile it once
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45 // #define that defines a constant (can't change it)

// Prototypes for functions
// notice how three of them take a pointer as an argument, per the *. we used to call this as a string
// essentially its just ex.: bool check(const string word);
// const says that those strings, when passed in arguments, must remain constant (i can´t change them)
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

#endif // DICTIONARY_H
