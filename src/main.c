#include "main.h" 
#include "exception.h"

int main() {
    
    try {
        raise(SIGSEGV);
        printf("SS\n");
        //maliciousInt = 100/maliciousInt;
    } catch {
        printf("Oh no!\n");
    }

    return 0;   
}
