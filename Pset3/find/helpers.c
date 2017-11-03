/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if ( n < 1 ) return false;
    for (int i = 0; i < n ; i++)
        if (values[i] == value) return true;
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{ // insertion sort
    for (int i = 0; i < n; i++)
        for (int j = i, s; j > 0 && values[j] < values[j-1]; j--) {
            s = values[j];
            values[j] = values[j-1];
            values[j-1] = s;
        }
    return;
}
