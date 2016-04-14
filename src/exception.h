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
    UNKNOWN_EXCEPTION               // No matching fault
} exceptions;


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

#define try setjmp(break_signal);while(catch_error(__LINE__))
#define catch(x) ;if(thrown_error(x))
#define finally ;if(no_exception())
#define retry revert_back()

#endif
