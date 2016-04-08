#ifndef _exception_handler
#define _exception_handler
#define _BSD_SOURCE

#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <setjmp.h>

enum exceptions {
    OK,
    NOTOK
};

int catchError(int currentLine);
int thrownError();

#define try while(catchError(__LINE__))
#define catch ;if(thrownError())

#endif
