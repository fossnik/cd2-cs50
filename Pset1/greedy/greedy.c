#include <stdio.h>
#include <cs50.h>

int main(void) {
    float owed;

    // verify that user has input a positive number
    do {
        printf("O hai! How much change is owed?:\n");
        owed = get_float();
    } while (!(owed > 0));

    // working with integers will be easier
    int cents_remain = owed * 100;

    // for keeping track of the total number of coins needed
    int coins = 0;

    // quarters to add to coins sum
    coins += cents_remain / 25;
    // the remainder is what's left
    cents_remain = cents_remain % 25;

    // dimes to add
    coins += cents_remain / 10;
    cents_remain = cents_remain % 10;

    // nickels to add
    coins += cents_remain / 5;
    cents_remain = cents_remain % 5;

    // pennies to add
    coins += cents_remain;

    // print result sufficed a newline character
    printf("%d\n", coins);
}