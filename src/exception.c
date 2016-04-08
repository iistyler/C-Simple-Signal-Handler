#include "exceptionHandler.h"

static int handleException = -1;

void handler(int sig) {
    handleException = 1;
}

int catchError(int currentLine) {
    signal(SIGSEGV, handler);

    static int line = -1;
    if (line == currentLine) {
        return 0;
    } else {
        line = currentLine;
        return 1;
    }
}

int thrownError() {
    if (handleException == 1)
        return 1;
    else
        return 0;
}


/* Allows user to print the exception to stderr */
void print_error( Error err ) {
        fprint_error(err, stderr);
}

/* Allows the user to print the exception to a given file stream 
 * Right now, only the user thrown exceptions will print an accurate line
 * number. The caught signals exceptions will not yet print an accurate line
 * 
 * Eventually:
 *  => Print the line the SIGSEGV occured at (backtrace()?)
 *  => Open the offending file and print the actual line to look pretty
 */
void fprint_error(Error err, FILE* stream) {
        fprintf(stream, "Exception %d thrown from function %s [%s:%d]\n", err.exception, \
                            err.function, err.file, err.line); 
}

