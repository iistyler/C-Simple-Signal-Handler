#include "main.h" 
#include "exception.h"

void test_unchecked();
void dealWithSegfaults();

int main() {
    int bad = 0;

    set_uncaught_exception(SEGFAULT_EXCEPTION, &dealWithSegfaults);

	//test_unchecked();
    create_exception(EXAMPLE_EXCEPTION);
    
    try {

        // Throw a segfault
        printf("Running okay..\n");
        //throw(EXAMPLE_EXCEPTION);
        float other = 1/bad;
        printf("Passed the exception!\n");
        //other = 10;

    } catch (DIVIDE_BY_ZERO_EXCEPTION) {

        // Divided by zero
        printf("Oh no a division by zero\n");
        bad = 1;
        retry;
    
    } catch (SEGFAULT_EXCEPTION) {

        // Recover from segfault
        printf("Oh no a segfault!\n");

    } catch (EXAMPLE_EXCEPTION) {

        printf("Test example\n");

    } finally {

        // No exception has occurred
        printf("State is OK\n");
    }

    /*try {
        printf("Continuing...\n");
    } finally {
        printf("No issue\n");
    }*/

    printf("Now let's continue...\n");

    return 0;
}

void dealWithSegfaults() {
    printf("User caught a segfault, exiting...\n");
    exit(1);
}

/* Test exceptions that occur outside the scope of a try block */
void test_unchecked() {
	raise(SIGSEGV);
    //raise(SIGFPE);
}
