// Credit

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void) {
    // long long is from the CS50 library
    long long plastic;
    string card = "INVALID";

    // sanity-check user input is purely numerical
    do {
        printf("Number:\n");
        plastic = get_long_long();
    } while (!(plastic > 0));

    long long cardnumber = plastic;
    long long remain;
    int sum = 0;
    bool even = true;

    // loop through the number until it is depleted
    while(plastic > 0){
        // digits are parsed in an alternating style
        if (even) {
            // EVENS //
            // using modulo thusly will render the last digit
            sum += plastic % 10;
        } else {
            // ODDS //
            // multiply product by two in accordance with Luhn's algorithm
            remain = (plastic % 10) * 2;
            // Luhn requires double digit numbers to be accounted for specially
            if (remain > 9) {
                // a number like 14 becomes a 1 and a 4.
                // it can never be more than 18 (2x of 9), so we can just add a 1.
                sum += remain % 10 + 1;
            } else {
                sum += remain;
            }
        }
        // flip parity
        even = !even;

        // cleave away the last digit
        plastic = plastic / 10;

        // test for different kinds of cards based on their initial digits
        if (plastic > 50 && plastic < 56) { card = "MASTERCARD"; }
        if (plastic == 34 || plastic == 37) { card = "AMEX"; }
        if (plastic == 4) { card = "VISA"; }
    }

    // checking if card has numbers consistant with use by the company
    if ( strcmp(card,"MASTERCARD") == 0 ) {
        // need 16 digits
        if ( cardnumber < 1000000000000000 || cardnumber > 9999999999999999) {
            card = "INVALID";
        }
    }
    if ( strcmp(card,"AMEX") == 0 ) {
        // need 15 digits
        if ( cardnumber < 100000000000000 || cardnumber > 999999999999999) {
            card = "INVALID";
        }
    }
    if ( strcmp(card,"VISA") == 0 ) {
        // need 13 or 16 digits
        // exclude greater than 16 or less than 13
        if ( cardnumber < 1000000000000 || cardnumber > 9999999999999999) {
            card = "INVALID";
        }
        // exclude less than 16 and greater than 13
        if ( cardnumber < 100000000000000 && cardnumber > 999999999999) {
            card = "INVALID";
        }
    }

    // The last digit of the sum must be a zero
    if (sum % 10 != 0) { card = "INVALID"; }

    // print result
    printf("%s\n", card);

    // they instructed me to return 0
    return 0;
}