/*                                                                            */
/*                 Title: C Exception handler                                 */
/*                 File: exception.c                                          */
/*                 Version: 1.0                                               */
/*                 Build Version 1.0                                          */
/*                 Last Modified: Aug 26, 2016                                */
/*                                                                            */

#include "exception.h"

/*************************** [ Static variables ] *****************************/

static exceptions handle_exception = OK;
static int pending_exception = 0;
static void (*signal_functions[10]) (int signal);

/************************* [ Function prototypes ] ****************************/

static void setup_signals();
static void reset_signal_defaults();
static void unchecked_handler(int sig);
static void handler (int sig);

/********************* [ Macro verification and setup ] ***********************/

#undef catch
#undef finally
#undef retry

/* If we are compiling on gcc, this will execute before main(), which allows 
 * us to catch any unchecked exceptions that occur before the first try block.
 * 
 * This relies on gcc as the __attribute__((constructor)) is a gcc construct. The
 * program should still compile with other compilers, however the program will 
 * not be able to catch any exception until the first try block is ran, setting up 
 * the signal handlers.
 *
 */
#ifdef USING_GCC

__attribute__((constructor)) void init_exceptions() {
    setup_signals();
    reset_signal_defaults();
}

#endif /* USING_GCC */


/************************* [ Signal configuration ] ***************************/

/**/
void setup_signals() {
    signal_functions[SEGFAULT_EXCEPTION]               = &unchecked_handler;
    signal_functions[DIVIDE_BY_ZERO_EXCEPTION]         = &unchecked_handler;
    signal_functions[ILLEGAL_INSTRUCTION_EXCEPTION]    = &unchecked_handler;
    signal_functions[BUS_ERROR_EXCEPTION]              = &unchecked_handler;
    signal_functions[ABORT_EXCEPTION]                  = &unchecked_handler;
    signal_functions[TRAP_EXCEPTION]                   = &unchecked_handler;
    signal_functions[SYS_CALL_EXCEPTION]               = &unchecked_handler;
    signal_functions[EMULATOR_TRAP_EXCEPTION]          = &unchecked_handler;
    signal_functions[UNKNOWN_EXCEPTION]                = &unchecked_handler;
}

/**/
void reset_signal_defaults() {
    signal(SIGSEGV, signal_functions[SEGFAULT_EXCEPTION]);
    signal(SIGFPE,  signal_functions[DIVIDE_BY_ZERO_EXCEPTION]);
    signal(SIGILL,  signal_functions[ILLEGAL_INSTRUCTION_EXCEPTION]);
    signal(SIGBUS,  signal_functions[BUS_ERROR_EXCEPTION]);
    signal(SIGABRT, signal_functions[ABORT_EXCEPTION]);
    signal(SIGIOT,  signal_functions[TRAP_EXCEPTION]);
    signal(SIGTRAP, signal_functions[SYS_CALL_EXCEPTION]);
    signal(SIGEMT,  signal_functions[EMULATOR_TRAP_EXCEPTION]);
    signal(SIGSYS,  signal_functions[UNKNOWN_EXCEPTION]);
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

/*********************** [ Global signal handling ] ***************************/

/**/
void handler(int sig) {
    pending_exception = 1;
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


/* 
 * Deals with all exceptions that occur outside of a try-catch block. 
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
void set_uncaught_exception(exceptions exceptionName, void(*forwardFunction)) {
    signal_functions[exceptionName] = forwardFunction;
    reset_signal_defaults();
}

/******************** [ Local catch block functions ] *************************/

/**/
void throw(int error) {
    handle_exception = error;
    longjmp(break_signal,1);
}

/**/
int is_checking_catch(int current_line) {
    static int line = -1;

    if (current_line == line) {
        line = -1;
        return 0;
    } else {
        line = current_line;
        return 1;
    }
}

/**/
int loop_through_exceptions(int current_line) {
    static int line = -1;
    static int times = 0;

    if (current_line == -1) {
        // RESET
        line = -1;
        times = 0;
        return 1;
    }

    if (line == current_line && times >= 2) {
        times = 0;
        reset_signal_defaults();

        if (pending_exception == 1 && handle_exception != OK)
            signal_functions[handle_exception](-1);
            //unchecked_handler(handle_exception);
        
        line = -1;
        return 0;

    } else if (times == 1) {
        times++;
        return 1;

    } else {

        // New try block
        times++;
        signal(SIGSEGV, handler);
        signal(SIGFPE,  handler);
        signal(SIGILL,  handler);
        signal(SIGBUS,  handler);
        signal(SIGABRT, handler);
        signal(SIGIOT,  handler);
        signal(SIGTRAP, handler);
        signal(SIGEMT,  handler);
        signal(SIGSYS,  handler);
        handle_exception = OK;
        pending_exception = 0;
        line = current_line;
        return 1;

    }
}

/**/
int catch(exceptions exception) {

    if (handle_exception == exception)
        pending_exception = 0;

    if (handle_exception == exception)
        return 1;
    else
        return 0;
}

/**/
int finally() {

    if (handle_exception == OK)
        return 1;
    else
        return 0;
}

/**/
void retry() {
    pending_exception = 0;
    handle_exception = OK;
    loop_through_exceptions(-1); // Reset
    longjmp(break_signal,1);
}

/**************************** [ Error Messaging ] *****************************/


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
