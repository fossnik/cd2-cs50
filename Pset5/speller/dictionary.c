/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"

// define the number of "buckets" (Singly Linked Lists)
static const unsigned int num_buckets = 50;

// wordcount counter variable
unsigned int wc = 0;

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
    // open input file (the dictionary)
    FILE *dict_p = fopen(dictionary, "r");
    if (dict_p == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    // create a buffer for storing individual words
    char *word = malloc(LENGTH);

    // create an array of nodes (node struct is defined in dictionary.h)
    node *hashtable[num_buckets];

    // set the heads of each to null.
    for (int i = 0; i < num_buckets; i++)
        hashtable[i] = NULL;

    // scan dictionary file line by line (ie. word by word)
    while( fscanf(dict_p, "%s", word) != EOF )
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
        unsigned int bucket = hasher(word);

        // avoid hash collisions - test that the bucket is empty
        if (hashtable[bucket] == NULL)
        {
            new_node->next = NULL;
            hashtable[bucket] = new_node;
        }
        else // whole chain gets tethered to new_node's next property.
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }

        // increment word count
        wc++;
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
unsigned int hasher(char *word)
{
    unsigned int hash_val = 1;

    // loop through each letter and perturb.
    // it's pretty random, but probably not random enough for certain uses.
    // most significantly, the correlation between function input and output
    // is fixed (returns the same result every time)
    for (int i = 0; word[i] != '\0'; i++)
        hash_val ^= word[i];

    hash_val = hash_val % (num_buckets - 1);

    return hash_val;
}