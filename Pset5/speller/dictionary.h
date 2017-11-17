/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * NODE TYPEDEF
 *  the node struct is self-referential
 *  's_linked_list' is an interim value that is relevant to
 *  defining self-referential structs, such as linked lists
 */
typedef struct s_linked_list
{
    char word[LENGTH + 1];
    struct s_linked_list* next;
}
node;

/**
 * Hash Function - XOR hash. Returns a hash as an int.
 */
unsigned int hasher(const char *word);

#endif // DICTIONARY_H
