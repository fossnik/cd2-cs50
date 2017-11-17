/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// define the number of hashtable "buckets" (Singly Linked Lists)
static const unsigned int hash_buckets = 50;

// wordcount counter variable
unsigned int wc = 0;

// create an array of nodes (node struct is defined in dictionary.h)
node *hashtable[hash_buckets];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // create a lowercase version of the test word using node struct
    // while const char *word is read only, a node is mutable
    node *temp_node = malloc(sizeof(node));

    // convert test word to small case
    for (int i = 0; word[i] != '\0'; i++)
        temp_node->word[i] = tolower(word[i]);

    // hash input to determine which bucket to search through
    unsigned int bucket = hasher(word);

    // "traversing linked lists" - seeking word
    // "node" is a struct defined in a dictionary.h typedef
    // iterate through all the words in the bucket, comparing strings
    for (node* ptr = hashtable[bucket]; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(ptr->word, temp_node->word) == 0)
        {
            free(temp_node);
            return true;
        }
    }

    free(temp_node);
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

    // set the heads of each to null.
    for (int i = 0; i < hash_buckets; i++)
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

        // convert to lower case for storage
        for (int i = 0; word[i] != '\0'; i++)
            word[i] = tolower(word[i]);

        // associate this word with the new node's word element
        strcpy(new_node->word, word);

        // hash the key (the word) to determine which bucket will keep the node
        unsigned int bucket = hasher(word);

        // avoid hash collisions - test that the bucket is empty
        if (hashtable[bucket] == NULL)
        {
            new_node->next = NULL;
            hashtable[bucket] = new_node;
        }
        // otherwise, the whole chain gets tethered to new_node's next property.
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }

        // increment word count
        wc++;
    }
    free(word);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // wc is 0 unless words are added to the dictionary.
    return wc;
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
unsigned int hasher(const char *word)
{
    unsigned int hash_val = 1;

    // loop through each letter and perturb.
    // it's pretty random, but probably not random enough for certain uses.
    // most significantly, the correlation between function input and output
    // is fixed (returns the same result every time)
    for (int i = 0; word[i] != '\0'; i++)
        hash_val ^= word[i];

    hash_val = hash_val % (hash_buckets - 1);

    return hash_val;
}