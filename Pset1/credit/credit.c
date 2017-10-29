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

    // start from second to last digit
    plastic = plastic / 10;

    while(plastic > 10){
        // use modulo to capture last digit in the number
        remain = plastic % 10;
        // multiply product by two in accordance with Luhn's algorithm
        remain = remain * 2;
        plastic = plastic / 100; // cleave away the last two digits

        // Luhn requires double digit numbers to be accounted for specially
        if (remain > 9) {
            // use modulo to cleave apart the second digit
            // a number like 14 becomes a 1 and a 4.
            // it can never be more than 18 (2x of 9), so we can just add a 1.
            sum += remain % 10 + 1;
        } else {
            sum += remain;
        }

        printf("%lld\n%s%lld%s%d\n", plastic, "RMDR: ", remain, " -|- Sum: ", sum);
    }

    printf("%s%d\n", "Sum: ", sum);
}