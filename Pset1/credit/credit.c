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
    printf("%s%lld\n\n", "INITIAL:\n", plastic);

    while(plastic > 1){
        // EVENS //
        // using modulo thusly will render the last digit
        sum += plastic % 10;
        // cleave away the last digit
        plastic = plastic / 10;

        // ODDS //
        // multiply product by two in accordance with Luhn's algorithm
        remain = (plastic % 10) * 2;
        // cleave away the last digit again
        plastic = plastic / 10;
        // Luhn requires double digit numbers to be accounted for specially
        if (remain > 9) {
            // use modulo to cleave apart the second digit
            // a number like 14 becomes a 1 and a 4.
            // it can never be more than 18 (2x of 9), so we can just add a 1.
            sum += remain % 10 + 1;
        } else {
            sum += remain;
        }
    }

    printf("%s%d\n", "Sum: ", sum);
}