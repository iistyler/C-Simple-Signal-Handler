#include "exception.h"

static exceptions handleException = OK;

exceptions switchToException (int sig) {
    exceptions currentException = OK;

    switch(sig) {
        case SIGSEGV:
            currentException = SEGFAULT_EXCEPTION;
            break;
        case SIGFPE:
            currentException = DIVIDE_BY_ZERO_EXCEPTION;
            break;
        case SIGILL:
            currentException = ILLEGAL_INSTRUCTION_EXCEPTION;
            break;
        case SIGBUS:
            currentException = BUS_ERROR_EXCEPTION;
            break;
        case SIGABRT:
            currentException = ABORT_EXCEPTION;
            break;
        case SIGTRAP:
            currentException = TRAP_EXCEPTION;
            break;
        case SIGEMT:
            currentException = EMULATOR_TRAP_EXCEPTION;
            break;
        case SIGSYS:
            currentException = SYS_CALL_EXCEPTION;
            break;
        default:
            currentException = UNKOWN_EXCEPTION;
            fprintf(stderr, "Unexpected exception occurred: %d\n", sig);
            break;
    }
    return currentException;
}

void handler(int sig) {
    exceptions currentException = OK;
    currentException = switchToException(sig);
    
    handleException = currentException;
    longjmp(breakSig,1);
}

int catchError(int currentLine) {
    signal(SIGSEGV, handler);
    signal(SIGFPE, handler);
    signal(SIGILL, handler);
    signal(SIGBUS, handler);
    signal(SIGABRT, handler);
    signal(SIGIOT, handler);
    signal(SIGTRAP, handler);
    signal(SIGEMT, handler);
    signal(SIGSYS, handler);

    static int line = -1;
    if (line == currentLine) {
        line = -1;
        return 0;
    } else {
        handleException = OK;
        line = currentLine;
        return 1;
    }
}

int thrownError(exceptions exception) {
    if (handleException == exception)
        return 1;
    else
        return 0;
}

int noException() {
    if (handleException == OK)
        return 1;
    else
        return 0;
}

void revertBack() {
    longjmp(breakSig,1);
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

/*void *buffer[100];
    int nptrs = 0;
    char **strings;

    nptrs = backtrace(buffer, 100);
    strings = backtrace_symbols(buffer, nptrs);
    printf("Exception thrown at: %s\n", strings[1]);*/
