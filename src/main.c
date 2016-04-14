#include "main.h" 
#include "exception.h"

int main() {
    int bad = 0;
    
    try {

        // Throw a segfault
        printf("Running okay..\n");
        float other = 1/bad;
        printf("Passed the exception!\n");

    } catch (DIVIDE_BY_ZERO_EXCEPTION) {

        // Divided by zero
        printf("Oh no a division by zero\n");
        bad = 1;
        retry;

    } catch (SEGFAULT_EXCEPTION) {

        // Recover from segfault
        printf("Oh no a segfault!\n");

    } finally {

        // No exception has occurred
        printf("State is OK\n");
    }

    printf("Now let's continue...\n");

    return 0;   
}
