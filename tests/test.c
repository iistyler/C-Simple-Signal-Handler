/*                                                                            */
/*                 Title: C Exception handler testing framework               */
/*                 File: test.h                                               */
/*                 Version: 1.0                                               */
/*                 Build Version 1.0                                          */
/*                 Last Modified: Aug 26, 2016                                */
/*                                                                            */

/* This is not part of the library and is just a series of unit tests */

#include "test.h"
#define NUMTESTS 5

void runTests();
void test1(FILE * compare, FILE * output);
void test2(FILE * compare, FILE * output);
void test3(FILE * compare, FILE * output);
void test4(FILE * compare, FILE * output);
void test5(FILE * compare, FILE * output);
int checkTest(int testNumber);
int checkTestResult();

void catchExit() {
    printf("Unexpected exit\n");
}

int main(int argc, char * argv[]) {

    // Set to hide details
    if (argc > 1)
        if (strcmp("-hideDetails", argv[1]) == 0)
            freopen("/dev/null", "w", stdout);

    // Run unit tests
    runTests();

    return 0;
}

void runTests() {
    int currentTest = 0;
    int total = 0;
    pid_t childpid;
    int status = 0;
    int fd[2];
    system("rm tests/err");
    int TT = 0;

    printf("\n");

    // Run each test
    for (int i = 1; i <= NUMTESTS; i++) {
        pipe(fd);
        currentTest = 0;
        printf("--------Running Test %d----------\n", i);

        childpid = fork();
        if (childpid == 0) {

            // Run test in child process
            currentTest = checkTest(i);

            close(fd[0]);
            write(fd[1], &currentTest, sizeof(currentTest));
            close(fd[1]);

            exit(0);
        } else {

            // Wait for test to finish
            waitpid(childpid,&status,0);

            close(fd[1]);
            read(fd[0], &currentTest, sizeof(currentTest));
            close(fd[0]);
        }

        // Display test info
        if (currentTest == 1) {
            printf("Test #%d Passed\n", i);
        } else {
            printf("Test #%d Failed\n", i);

            // Add error define for compilation
            FILE * errFile = fopen("tests/err", "a+");
            fprintf(errFile, "-D T%d ", i);
            fclose(errFile);
        }
        printf("--------------------------------\n\n");
        total += currentTest;
    }

    if (total == NUMTESTS)
        printf("Library tests PASSED, %d/%d tests completed successfully\n", total, NUMTESTS);
    else
        printf("Library tests FAILED, %d/%d tests completed successfully\n", total, NUMTESTS);
}

int checkTest(int testNumber) {

    int passed = 0;
    
    FILE * compareFile = fopen("tests/compare.ref","w+");
    FILE * outputFile = fopen("tests/output.ref","w+");

    if (compareFile == NULL) {
        printf("Could not open file 'compare.ref'\n");
        exit(1);
    }
    if (outputFile == NULL) {
        printf("Could not open file 'output.ref'\n");
        exit(1);
    }

    switch (testNumber) {
        case 1:     test1(compareFile, outputFile);     break;
        case 2:     test2(compareFile, outputFile);     break;
        case 3:     test3(compareFile, outputFile);     break;
        case 4:     test4(compareFile, outputFile);     break;
        case 5:     test5(compareFile, outputFile);     break;
        default:
            printf("Test %d not found\n", testNumber);  break;
    }

    fprintf(compareFile, "\n");
    fprintf(outputFile, "\n");
    fclose(compareFile);
    fclose(outputFile);
    passed = checkTestResult();

    return passed;
}

int checkTestResult() {
    int passed = 1;
    system("diff tests/compare.ref tests/output.ref > tests/diff.ref");
    char buffer[1000] = { 0 };
    int foundErr = 0;
    FILE * diffFile = fopen("tests/diff.ref","r");

    while(fgets(buffer, 999, diffFile) != NULL) {
        passed = 0;
        if (foundErr == 0) {
            printf("Test differs from expected:\n");
            foundErr = 1;
        } else {
            printf("%s\n", buffer);
        }
    }
    if (strlen(buffer) > 0)
        passed = 0;

    fclose(diffFile);

    return passed;
}

// Check no exception
void test1(FILE * compare, FILE * output) {
    printf("Testing no exception\n");

    try {
        fprintf(output,"1");
    } catch(SEGFAULT_EXCEPTION) {
        fprintf(output,"2");
    } finally {
        fprintf(output,"3");
    }
    fprintf(output,"4");

    // Expected result
    fprintf(compare, "134");
}

// Check divide by zero
void test2(FILE * compare, FILE * output) {
    printf("Testing division by zero\n");
    int bad = 0;

    try {
        fprintf(output,"0");
        float test = 1/bad;
        fprintf(output,"1");
        test = 2;
    } catch(SEGFAULT_EXCEPTION) {
        fprintf(output,"2");
    } catch(DIVIDE_BY_ZERO_EXCEPTION) {
        fprintf(output,"3");
    } finally {
        fprintf(output,"4");
    }
    fprintf(output,"5");

    // Expected result
    fprintf(compare, "035");
}

// Check retry
void test3(FILE * compare, FILE * output) {
    printf("Testing rety\n");
    int bad = 0;

    try {
        fprintf(output,"0");
        float test = 1/bad;
        fprintf(output,"1");
        test = 2;
    } catch(SEGFAULT_EXCEPTION) {
        fprintf(output,"2");
    } catch(DIVIDE_BY_ZERO_EXCEPTION) {
        fprintf(output,"3");
        bad = 1;
        fprintf(output,"6");
        retry;
        fprintf(output,"7");
    } finally {
        fprintf(output,"4");
    }
    fprintf(output,"5");

    // Expected result
    fprintf(compare, "0360145");
}

// Check throw
void test4(FILE * compare, FILE * output) {
    printf("Testing throw\n");

    try {
        fprintf(output,"0");
        throw(SEGFAULT_EXCEPTION);
        fprintf(output,"1");
    } catch(SEGFAULT_EXCEPTION) {
        fprintf(output,"2");
    } catch(DIVIDE_BY_ZERO_EXCEPTION) {
        fprintf(output,"3");
    } finally {
        fprintf(output,"4");
    }
    fprintf(output,"5");

    // Expected result
    fprintf(compare, "025");
}

// Check custom exception
void test5(FILE * compare, FILE * output) {
    printf("Testing custom exceptions\n");

    create_exception(TEST_EXCEP);

    try {
        fprintf(output,"0");
        throw(TEST_EXCEP);
        fprintf(output,"1");
    } catch(SEGFAULT_EXCEPTION) {
        fprintf(output,"2");
    } catch(TEST_EXCEP) {
        fprintf(output,"3");
    } finally {
        fprintf(output,"4");
    }
    fprintf(output,"5");

    // Expected result
    fprintf(compare, "035");
}
