#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Check for ANSI C compliance */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/* Global objects - variables (used in other codes as external) */
BufferPointer stringLiteralTable; /* This buffer implements String Literal Table */
BufferPointer contentBuffer;
corex_intg errorNumber;           /* Run-time error number = 0 by default (ANSI) */

/* External objects */
extern corex_intg line; /* Source code line numbers - defined in Scanner.c */
extern Token tokenizer(corex_void);

/* Function declarations */
corex_void printScannerError(corex_string fmt, ...);
corex_void displayScanner(BufferPointer ptrBuffer);
corex_long getScannerFilesize(corex_string fname);
corex_void printToken(Token t);
corex_void printStringTable();

/*
************************************************************
 *  Scanner Main function
 * Parameters:
 *   argc / argv = Parameters from command prompt
 * Return value:
 *	Success operation.
 ***********************************************************
 */

corex_intg mainScanner(corex_intg argc, corex_string* argv) {
    BufferPointer sourceBuffer; /* Pointer to input (source) buffer */
    FILE* fileHandler;          /* Input file handle */
    Token currentToken;         /* Token produced by the scanner */
    corex_intg loadSize = 0;    /* The size of the file loaded in the buffer */

    /* Check for correct arguments - source file name */
    if (argc <= 2) {
        /* __DATE__, __TIME__, __LINE__, __FILE__ are predefined preprocessor macros */
        printScannerError("Date: %s  Time: %s", __DATE__, __TIME__);
        printScannerError("Runtime error at line %d in file %s", __LINE__, __FILE__);
        printScannerError("%s%s", argv[0], ": Missing source file name.");
        printScannerError("%s", "Usage: <Option=1> <SourceFile>");
        exit(EXIT_FAILURE);
    }

    /* Shows debug mode */
    printf("%s%d%s", "[Debug mode: ", DEBUG, "]\n");

    /* Create a source code input buffer - multiplicative mode */
    sourceBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_MULTI);
    if (sourceBuffer == NULL) {
        printScannerError("%s%s", argv[1], ": Could not create source buffer");
        exit(EXIT_FAILURE);
    }

    /* Open source file */
    if ((fileHandler = fopen(argv[2], "r")) == NULL) {
        printScannerError("%s%s%s", argv[0], ": Cannot open file: ", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* Load source file into input buffer */
    printf("Reading file %s ....Please wait\n", argv[2]);
    loadSize = readerLoad(sourceBuffer, fileHandler);
    if (loadSize == READER_ERROR)
        printScannerError("%s%s", argv[0], ": Error in loading buffer.");

    /* Close source file */
    fclose(fileHandler);

    /* Find the size of the file */
    if (loadSize == READER_ERROR) {
        printf("The input file %s %s\n", argv[2], "is not completely loaded.");
        printf("Input file size: %ld\n", getScannerFilesize(argv[2]));
    }

    /* Compact and display the source buffer and add SEOF to input program buffer */
    if ((loadSize != READER_ERROR) && (loadSize != 0)) {
        if (readerAddChar(sourceBuffer, READER_TERMINATOR)) {
            displayScanner(sourceBuffer);
        }
    }

    /* Create string Literal Table */
    stringLiteralTable = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_ADDIT);
    if (stringLiteralTable == NULL) {
        printScannerError("%s%s", argv[0], ": Could not create string literals buffer");
        exit(EXIT_FAILURE);
    }

    /* Create content Table */
    contentBuffer = readerCreate(READER_DEFAULT_SIZE, READER_DEFAULT_INCREMENT, MODE_ADDIT);
    if (contentBuffer == NULL) {
        printScannerError("%s%s", argv[0], ": Could not create content literals buffer");
        exit(EXIT_FAILURE);
    }

    /* Testbed for the scanner and add SEOF to input program buffer */
    /* Initialize scanner input buffer */
    if (startScanner(sourceBuffer)) {
        printScannerError("%s%s", argv[0], ": Empty program buffer - scanning canceled");
        exit(EXIT_FAILURE);
    }

    printf("\nScanning source file...\n\n");
    printf("Token\t\tAttribute\n");
    printf("----------------------------------\n");
    do {
        currentToken = tokenizer();
        printToken(currentToken);
    } while (currentToken.code != SEOF_T);

    /* Print String Literal Table if not empty */
    printStringTable();

    readerRestore(sourceBuffer);
    readerRestore(stringLiteralTable);
    readerRestore(contentBuffer);
    sourceBuffer = stringLiteralTable = contentBuffer = NULL;
    printScannerData(scData);
    /* Ass2 evaluation only */
    if (argv[3] != NULL && *argv[3] == 'l')
        printf("The number of lines is: %d\n", line);

    return (EXIT_SUCCESS);
}

/*
************************************************************
 *  Error printing function with variable number of arguments
 *  Params: Variable arguments, using formats from C language.
 *	 - Internal vars use list of arguments and types from stdarg.h
 *   - NOTE: The format is using signature from C Language
***********************************************************
 */

corex_void printScannerError(corex_string fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    (void)vfprintf(stderr, fmt, ap);
    va_end(ap);
    /* Move to new line */
    if (strchr(fmt, '\n') == NULL)
        fprintf(stderr, "\n");
}

/*
************************************************************
 * The function displays buffer contents
 * Param:
 *		- Scanner to be displayed.
 ***********************************************************
 */

corex_void displayScanner(BufferPointer ptrBuffer) {
    printf("\nPrinting buffer parameters:\n\n");
    printf("The capacity of the buffer is:  %d\n", readerGetSize(ptrBuffer));
    printf("The current size of the buffer is:  %d\n", readerGetPosWrte(ptrBuffer));
    printf("\nPrinting buffer contents:\n\n");
    readerRecover(ptrBuffer);
    readerPrint(ptrBuffer);
}

/*
 ************************************************************
 * The function gets size of scanner file
 * Param:
 *	- Filename
 * Return:
 *	- Size of the file
 ***********************************************************
 */

corex_long getScannerFilesize(corex_string fname) {
    FILE* fileInput;
    corex_long fileLength;
    fileInput = fopen(fname, "r");
    if (fileInput == NULL) {
        printScannerError("%s%s", "Cannot open file: ", fname);
        return 0L;
    }
    fseek(fileInput, 0L, SEEK_END);
    fileLength = ftell(fileInput);
    fclose(fileInput);
    return fileLength;
}

/*
************************************************************
 * Print String Table
 * Param:
 *		- None
 ***********************************************************
 */

corex_void printStringTable() {
    printf("\nPrinting string table...\n");
    printf("----------------------------------\n");
    corex_intg pos = readerGetPosWrte(stringLiteralTable);
    if (pos > 0) {
        corex_string str = readerGetContent(stringLiteralTable, 0);
        corex_intg i = 0;
        corex_intg start = 0;
        corex_boln inStrLiteral = COREX_FALSE;

        while (i <= pos) {
            if (str[i] == CHARSEOF0) {
                if (inStrLiteral) {
                    // Check if the current string literal is a comment and skip printing it
                    corex_intg length = i - start;
                    corex_char* currentStr = &str[start];
                    printf("%.*s", length, currentStr);
                    inStrLiteral = COREX_FALSE;
                }
                start = i + 1;
            }
            else {
                if (!inStrLiteral) {
                    inStrLiteral = COREX_TRUE;
                    start = i;
                }
            }
            i++;
        }
        
    }
    printf("\n----------------------------------\n");
}


