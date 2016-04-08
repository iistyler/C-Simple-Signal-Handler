#include "main.h" 
#include "exception.h"

int main() {
    
    try {

        // Throw a segfault
        raise(SIGSEGV);
        printf("SS\n");

    } catch (UNKOWN_EXCEPTION) {

        // Other exception
        printf("Unexpected error has occurred\n");
        
    } catch (SEGFAULT_EXCEPTION) {

        // Recover from segfault
        printf("Oh no!\n");

    } finally {

        // No exception has occurred
        printf("State is OK\n");
    }

    return 0;   
}
