#include "exception.h"

static exceptions handle_exception = OK;
static void (*signalFunctions[10]) (int signal);

void handler(int sig);
void setupSignals();
void resetSignals();

/* If we are compiling on gcc, this will execute before main(), which allows 
 * us to catch any unchecked exceptions that occur before the first try block.
 * 
 * This relies on gcc as the __attribute__((constructor)) is a gcc construct. The
 * program should still compile with other compilers, however the program will 
 * not be able to catch any exception until the first try block is ran, setting up 
 * the signal handlers.
 *
 */
__attribute__((constructor)) void init_exceptions() {
    setupSignals();
    resetSignals();
}


/**/
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

void set_uncaught_exception(exceptions exceptionName, void(*forwardFunction)) {
    signalFunctions[exceptionName] = forwardFunction;
    resetSignals();
}

/**/
void setupSignals() {
    signalFunctions[SEGFAULT_EXCEPTION] =               &unchecked_handler;
    signalFunctions[DIVIDE_BY_ZERO_EXCEPTION] =         &unchecked_handler;
    signalFunctions[ILLEGAL_INSTRUCTION_EXCEPTION] =    &unchecked_handler;
    signalFunctions[BUS_ERROR_EXCEPTION] =              &unchecked_handler;
    signalFunctions[ABORT_EXCEPTION] =                  &unchecked_handler;
    signalFunctions[TRAP_EXCEPTION] =                   &unchecked_handler;
    signalFunctions[SYS_CALL_EXCEPTION] =               &unchecked_handler;
    signalFunctions[EMULATOR_TRAP_EXCEPTION] =          &unchecked_handler;
    signalFunctions[UNKNOWN_EXCEPTION] =                &unchecked_handler;
}

/**/
void resetSignals() {
    signal(SIGSEGV, signalFunctions[SEGFAULT_EXCEPTION]);
    signal(SIGFPE,  signalFunctions[DIVIDE_BY_ZERO_EXCEPTION]);
    signal(SIGILL,  signalFunctions[ILLEGAL_INSTRUCTION_EXCEPTION]);
    signal(SIGBUS,  signalFunctions[BUS_ERROR_EXCEPTION]);
    signal(SIGABRT, signalFunctions[ABORT_EXCEPTION]);
    signal(SIGIOT,  signalFunctions[TRAP_EXCEPTION]);
    signal(SIGTRAP, signalFunctions[SYS_CALL_EXCEPTION]);
    signal(SIGEMT,  signalFunctions[EMULATOR_TRAP_EXCEPTION]);
    signal(SIGSYS,  signalFunctions[UNKNOWN_EXCEPTION]);
}

/**/
void handler(int sig) {
    exceptions current_exception = OK;
    current_exception = switch_to_exception(sig);
    handle_exception = current_exception;
    longjmp(break_signal,1);

	/* If the user want's core dumps then this needs to execute: */
/*
#ifdef CORE_DUMP
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
#endif
*/
}

/* Deals with all exceptions that occur outside of a try-catch block. 
 * This function will print out the stack trace, then exit the program as 
 * there is not much else to do. 
 *
 * For this to work, need to set up some way to revert the signal handler to 
 * this function after the end of a try-catch block. 
 */
void unchecked_handler(int sig) {
	printf("Got unchecked exception, exiting...\n");
	exit(EXIT_FAILURE);
}

/**/
void throw(int error) {
    handle_exception = error;
    longjmp(break_signal,1);
}

/**/
int catch_error(int current_line) {
    static int line = -1;

    if (line == current_line) {

        signal(SIGSEGV, handler);
        signal(SIGFPE,  handler);
        signal(SIGILL,  handler);
        signal(SIGBUS,  handler);
        signal(SIGABRT, handler);
        signal(SIGIOT,  handler);
        signal(SIGTRAP, handler);
        signal(SIGEMT,  handler);
        signal(SIGSYS,  handler);
        line = -1;
        return 0;

    } else {

        resetSignals();
        handle_exception = OK;
        line = current_line;
        return 1;

    }
}

/**/
int thrown_error(exceptions exception) {

    if (handle_exception == exception)
        return 1;
    else
        return 0;
}

/**/
int no_exception() {

    if (handle_exception == OK)
        return 1;
    else
        return 0;
}

/**/
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

/* Print out the stack trace */
void print_stacktrace () {


}

/*void *buffer[100];
    int nptrs = 0;
    char **strings;

    nptrs = backtrace(buffer, 100);
    strings = backtrace_symbols(buffer, nptrs);
    printf("Exception thrown at: %s\n", strings[1]);*/
