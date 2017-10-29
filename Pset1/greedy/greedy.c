#include <stdio.h>
#include <cs50.h>

int main(void) {
    // float owed;

    // // check for positive
    // do {
    //     printf("O hai! How much change is owed?: ");
    //     owed = get_float();
    // } while (!(owed > 0));

    // calculate coins needed for change
    float owed = 0.41;
    int cents_remain = owed * 100;
    int coins = 0;

    // number of quarters to add to sum total of coins
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

    // print result
    printf("%d\n", coins);
}