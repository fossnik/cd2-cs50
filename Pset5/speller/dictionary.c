/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"

// define the size of the hash table array
static const unsigned int array_size = 50;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open input file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // create a buffer for storing individual words
    char *word = malloc(LENGTH);

    // create an array of nodes (by the node struct defined in dictionary.h)
    node *hashtable[array_size] = NULL;

    // scan dictionary file line by line (ie. word by word)
    while( fscanf(inptr, "%s", word) != EOF )
    {
        // malloc a node pointer for each new word
        node *new_node = malloc(sizeof(node));

        // test that the new node is not null
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // associate this word with the new node's word element
        strcpy(new_node->word, word);

        // hash the key (the word) to determine where to place the new node.
        unsigned int bucket = hasher(word, array_size);

        // point node to the current HEAD of the array
        new_node->next = hashtable[bucket];

        // store node as the new head of the array
        hashtable[bucket] = new_node;
    }
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}

/**
 * Hash Function - XOR hash. Returns a hash as an int.
 */
unsigned int hasher(char *word, unsigned int array_length)
{
    unsigned int hash_val = 1;

    // loop through each letter and perturb.
    for (int i = 0; word[i] != '\0'; i++)
        hash_val ^= word[i];

    hash_val = hash_val % 50;

    return hash_val;
}