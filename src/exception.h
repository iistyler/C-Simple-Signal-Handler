/*                                                                            */
/*                 Title: C Exception handler                                 */
/*                 File: exception.h                                          */
/*                 Version: 1.0                                               */
/*                 Build Version 1.0                                          */
/*                 Last Modified: May 5, 2016                                 */
/*                                                                            */

#ifndef _exception_handler
#define _exception_handler
#define _BSD_SOURCE

/*************************** [ External libraries ] ***************************/

#include <stdio.h>              // Input/Output
#include <stdlib.h>             // General Utilities
#include <signal.h>             // Signals
#include <setjmp.h>             // longjumps

/* Depricated includes (Seemingly unused) */
//#include <execinfo.h>
//#include <pthread.h>
//#include <unistd.h>
//#include <string.h>

/********************* [ Structure and variable setup ] ***********************/

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
    int exception;                  // The exception number
    int line;                       // Line number it happened on
    char* function;                 // Function the error was thrown
    char* file;                     // The file the error was thrown in
} Error;

/* Interfaces for printing the errors that occured */
void print_error (Error err);
void fprint_error (Error err, FILE * stream);

/*************************** [ External functions ] ***************************/

extern int loop_through_exceptions(int current_line);
extern int catch(exceptions exception);
extern int finally();
extern void retry();
extern void throw(int error);
extern int is_checking_catch(int current_line);
extern void set_uncaught_exception(exceptions exceptionName, void(*forwardFunction));

/***************************** [ Macro setup ] ********************************/

#define try setjmp(break_signal);while(loop_through_exceptions(__LINE__))if(is_checking_catch(__LINE__))
#define catch(x) else if(catch(x))
#define finally else if(finally())
#define retry retry()
#define create_exception(x) int x = __LINE__+50 // THIS LINE NEEDS REFACTORING


/************************* [ Macro error handling ] ***************************/

/* 
 * Trying out putting the signal handlers here to allow us to catch unchecked 
 * exceptions. If they are only in loop_through_exceptions, and a signal is raised before
 * that function is called then nothing will happen.
 *
 * This constructor is a gcc macro, so only use when compiling with GCC
 */
#ifdef __clang__

#warning Some features are not available when compiling with clang

#else

#define USING_GCC
void init_exceptions() __attribute__((constructor));

#endif /* __clang__ */


#endif /* _exception_handler */
