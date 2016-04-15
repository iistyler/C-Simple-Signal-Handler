#include "main.h" 
#include "exception.h"

void test_unchecked();

int main() {
    int bad = 0;
	
	test_unchecked();
    create_exception(EXAMPLE_EXCEPTION);
    
    try {
        // Throw a segfault
        printf("Running okay..\n");
        throw(EXAMPLE_EXCEPTION);
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

    } catch (EXAMPLE_EXCEPTION) {

        printf("Test example\n");

    } finally {

        // No exception has occurred
        printf("State is OK\n");
    }

    printf("Now let's continue...\n");

    return 0;   
}


/* Test exceptions that occur outside the scope of a try block */
void test_unchecked() {
	raise(SIGSEGV);
}
