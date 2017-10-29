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
    int owedCents = owed * 100;
    int coins = 0;
    int remain;

    // number of quarters to add to sum total of coins
    coins += owedCents / 25;
    // what's leftover goes to remain
    remain = owedCents % 25;

    // dimes to add
    coins += remain / 10;
    remain = remain % 10;

    // nickels to add
    coins += remain / 5;
    remain = remain % 5;

    // pennies to add
    coins += remain;

    // print result
    printf("%d\n", coins);
}