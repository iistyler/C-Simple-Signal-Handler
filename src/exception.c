#include "exception.h"

static exceptions handle_exception = OK;

exceptions switch_to_exception (int sig) {
    exceptions current_exception = OK;

    switch(sig) {
        case SIGSEGV:   current_exception = SEGFAULT_EXCEPTION;             break;
        case SIGFPE:    current_exception = DIVIDE_BY_ZERO_EXCEPTION;       break;
        case SIGILL:    current_exception = ILLEGAL_INSTRUCTION_EXCEPTION;  break;
        case SIGBUS:    current_exception = BUS_ERROR_EXCEPTION;            break;
        case SIGABRT:   current_exception = ABORT_EXCEPTION;                break;
        case SIGTRAP:   current_exception = TRAP_EXCEPTION;                 break;
        case SIGSYS:    current_exception = SYS_CALL_EXCEPTION;             break;        
        case SIGEMT:    current_exception = EMULATOR_TRAP_EXCEPTION;        break;
        default:        current_exception = UNKNOWN_EXCEPTION;
            fprintf(stderr, "Unexpected exception occurred: %d\n", sig);    break;
    }
    return current_exception;
}

void handler(int sig) {
    exceptions current_exception = OK;
    current_exception = switch_to_exception(sig);
    handle_exception = current_exception;
    longjmp(break_signal,1);
}

void throw(int error) {
    handle_exception = error;
    longjmp(break_signal,1);
}

int catch_error(int current_line) {
    static int line = -1;

    signal(SIGSEGV, handler);
    signal(SIGFPE, handler);
    signal(SIGILL, handler);
    signal(SIGBUS, handler);
    signal(SIGABRT, handler);
    signal(SIGIOT, handler);
    signal(SIGTRAP, handler);
    signal(SIGEMT, handler);
    signal(SIGSYS, handler);

    if (line == current_line) {
        line = -1;
        return 0;
    } else {
        handle_exception = OK;
        line = current_line;
        return 1;
    }
}

int thrown_error(exceptions exception) {
    if (handle_exception == exception)
        return 1;
    else
        return 0;
}

int no_exception() {
    if (handle_exception == OK)
        return 1;
    else
        return 0;
}

void revert_back() {
    longjmp(break_signal,1);
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
