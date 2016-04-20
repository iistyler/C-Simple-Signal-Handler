/* This is not part of the library and is just a series of automated tests */

#ifndef _TESTS_MAIN
#define _TESTS_MAIN
#define _BSD_SOURCE

#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <execinfo.h>

#ifndef SIGSEGV
#warning SEGFAULT_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGFPE
#warning DIVIDE_BY_ZERO_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGILL
#warning ILLEGAL_INSTRUCTION_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGBUS
#warning BUS_ERROR_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGABRT
#warning ABORT_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGTRAP
#warning TRAP_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGEMT
#warning EMULATOR_TRAP_EXCEPTION cannot be tested on this OS
#endif

#ifndef SIGSYS
#warning SYS_CALL_EXCEPTION cannot be tested on this OS
#endif

#include "../src/exception.h"

#endif
