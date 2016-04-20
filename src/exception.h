#ifndef _exception_handler
#define _exception_handler
#define _BSD_SOURCE

#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <execinfo.h>
#include <pthread.h>

jmp_buf break_signal;

typedef enum exceptions {
    OK,
    SEGFAULT_EXCEPTION,            // SIGSEGV
    DIVIDE_BY_ZERO_EXCEPTION,      // SIGFPE
    ILLEGAL_INSTRUCTION_EXCEPTION, // SIGILL
    BUS_ERROR_EXCEPTION,           // SIGBUS
    ABORT_EXCEPTION,               // SIGABRT
    TRAP_EXCEPTION,                // SIGTRAP
    EMULATOR_TRAP_EXCEPTION,       // SIGEMT
    SYS_CALL_EXCEPTION,            // SIGSYS
    UNKNOWN_EXCEPTION              // No matching fault
} exceptions;

/* Ran into issues with this not being defined in some places */
#ifndef SIGEMT
#define SIGEMT -1
#endif


/* 
 * Format for defining the exception that occured, and allow the 
 * user to access information about the exception that occured
 *
 */
typedef struct Error {
    int exception;
    int line;
    char* function;
    char* file;
} Error;

/* Interfaces for printing the errors that occured */
void print_error (Error err);
void fprint_error (Error err, FILE * stream);


int catch_error(int current_line);
int thrown_error(exceptions exception);
int no_exception();
void revert_back();
void throw(int error);
void handler (int sig);
int check_catch(int current_line);
void unchecked_handler(int sig);
extern void set_uncaught_exception(exceptions exceptionName, void(*forwardFunction));

#define try setjmp(break_signal);while(catch_error(__LINE__))if(check_catch(__LINE__))
#define catch(x) else if(thrown_error(x))
#define finally else if(no_exception())
#define retry revert_back()
#define create_exception(x) int x = __LINE__+50


/* 
 * Trying out putting the signal handlers here to allow us to catch unchecked 
 * exceptions. If they are only in catch_error, and a signal is raised before
 * that function is called then nothing will happen.
 *
 * This constructor is a gcc macro, so only use when compiling with GCC
 */
#ifdef __GNUC__

void init_exceptions() __attribute__((constructor));

#endif /* __GNUC__ */


#endif /* _exception_handler */
