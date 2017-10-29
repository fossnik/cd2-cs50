// Credit

#include <stdio.h>
#include <cs50.h>

int main(void) {
    // long long is from the CS50 library
    long long plastic;

    // sanity-check user input is purely numerical
    // do {
    //     printf("Number:\n");
    //     plastic = get_long_long();
    // } while (!(plastic > 0));


    plastic = 378282246310005;
    long long remain;
    int sum = 0;
    // string card = "INVALID";
    bool even = true;

    while(plastic > 0){
        // the if statement makes it only process even or odd
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
    }

    printf("%s%d\n", "Sum: ", sum);

    // they instructed me to return 0
    return 0;
}