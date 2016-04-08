#ifndef _exception_handler
#define _exception_handler
#define _BSD_SOURCE

#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <string.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf breakSig;

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
    UNKOWN_EXCEPTION               // No matching fault
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


int catchError(int currentLine);
int thrownError(exceptions exception);
int noException();

#define try setjmp(breakSig);while(catchError(__LINE__))
#define catch(x) ;if(thrownError(x))
#define finally ;if(noException())

#endif
