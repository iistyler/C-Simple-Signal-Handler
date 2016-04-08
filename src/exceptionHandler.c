#include "exceptionHandler.h"

static int handleException = -1;

void handler(int sig) {
    handleException = 1;
}

int catchError(int currentLine) {
    signal(SIGSEGV, handler);

    static int line = -1;
    if (line == currentLine) {
        return 0;
    } else {
        line = currentLine;
        return 1;
    }
}

int thrownError() {
    if (handleException == 1)
        return 1;
    else
        return 0;
}
