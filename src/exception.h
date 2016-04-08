#ifndef _exception_handler
#define _exception_handler
#define _BSD_SOURCE

#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <string.h>
#include <signal.h>
#include <setjmp.h>

enum exceptions {
    OK,
    NOTOK
};


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
void fprint_error (Error err);


int catchError(int currentLine);
int thrownError();

#define try while(catchError(__LINE__))
#define catch ;if(thrownError())

#endif
