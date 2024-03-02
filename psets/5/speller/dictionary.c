// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

// Hash table
node *table[N];

// Size of the dictionary
int size_d = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    for (int i = 0; i < N; i++)
    {
        for (node *str = table[i]; str != NULL; str = str->next)
        {
            if (!strcasecmp(word, str->word))
            {
                return true;
            }
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    size_d++;

    // Improve this hash function with bitwise XOR
    return toupper(word[0]) ^ 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *d = fopen(dictionary, "r");
    if (d == NULL)
    {
        return false;
    }

    char str[LENGTH + 1];
    while (fgets(str, LENGTH + 2, d))
    {
        int hash_i = hash(str);

        node *n = malloc(sizeof(node));

        for (int i = 0; i < LENGTH + 1; i++)
        {
            if (str[i] == '\n')
            {
                n->word[i] = '\0';
                n->next = NULL;

                n->next = table[hash_i];
                table[hash_i] = n;

                break;
            }
            n->word[i] = str[i];
        }
    }

    fclose(d);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return size_d;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];
        while (n != NULL)
        {
            node *next = n->next;
            free(n);
            n = next;
        }
    }

    return true;
}
