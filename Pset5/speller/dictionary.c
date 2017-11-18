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
static const unsigned int hash_buckets = 5;

// wordcount tracker
unsigned int wc = 0;

// this array of node pointers keeps track of each linked list's HEAD node
// (node struct is type defined in dictionary.h)
node *sll_head_table[hash_buckets];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // while the given const char *word is read-only, a node struct is mutable
    // a temporary node shall store the small-case transmogrification of 'word'
    node *temp_node = malloc(sizeof(node));

    // convert test word to small case
    for (int i = 0; word[i] != '\0'; i++)
        temp_node->word[i] = tolower(word[i]);

    // hash input to determine which bucket to search through
    unsigned int bucket = hasher(word);

    // seeking 'word' match, parse each word element in the selected node chain
    for (node* dict_node = sll_head_table[bucket]; dict_node != NULL; dict_node = dict_node->next)
    {
        printf("WORD: %s    \ttemp->WORD: %s    \tdict->WORD: %s    \t(buckets: %d %d %d)\n", word, temp_node->word, dict_node->word, hasher(word), hasher(dict_node->word), hasher(temp_node->word));
        if (strcmp(dict_node->word, temp_node->word) == 0)
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
    char a_word[LENGTH];

    // set NULL each index in the hash table, signifying absence of list HEADs
    for (int i = 0; i < hash_buckets; i++)
        sll_head_table[i] = NULL;

    // parse dictionary file line by line (ie. word by word)
    while( fscanf(dict_p, "%s", a_word) != EOF )
    {
        // malloc a node pointer for each new word
        node *new_node = malloc(sizeof(node));

        // test that the new node is not null
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // convert to lower case (mutable array)
        for (int i = 0; a_word[i] != '\0'; i++)
            a_word[i] = tolower(a_word[i]);

        // associate this word with the new node's word element (string copy)
        strcpy(new_node->word, a_word);

        // hash the word (key) to determine which bucket ought keep the node
        unsigned int bucket = hasher(new_node->word);

        // Mediation of Hash Collisions
        // if this index of our buckets array has not been furnished with a
        // sll HEAD node, then let new_node's NEXT element indicate NULL, and
        // register new_node's location in memory at the said index.
        if (sll_head_table[bucket] == NULL)
        {
            new_node->next = NULL;
            sll_head_table[bucket] = new_node;
        }
        // otherwise, the whole chain gets tethered to new_node's NEXT property.
        // new_node is then registered as the new HEAD of the linked list
        else
        {
            new_node->next = sll_head_table[bucket];
            sll_head_table[bucket] = new_node;
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

    // derive a numerical output that's contingent on each letter of the input.
    // for some use cases, this type of hash function is insufficiently random,
    // but, most significantly, the fixed correlation between input and output
    // ensures that a given word input always returns the same number as output
    for (int i = 0; word[i] != '\0'; i++)
        hash_val ^= word[i];

    hash_val = hash_val % (hash_buckets - 1);

    return hash_val;
}