/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
*/

#ifndef COMPILERS_H
#define COMPILERS_H

#define DEBUG 0

/* Language name */
#define STR_LANGNAME    "CoreX"

/* Logical constants */
#define COREX_TRUE  1
#define COREX_FALSE 0

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/

typedef char            corex_char;
typedef char*           corex_string;
typedef int             corex_intg;
typedef float           corex_real;
typedef void            corex_void;

typedef unsigned char   corex_boln;
typedef unsigned char   corex_byte;

typedef long            corex_long;
typedef double          corex_doub;

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
    PGM_READER = 'r',
    PGM_SCANNER = 's',
    PGM_PARSER = 'p'
};

/*
------------------------------------------------------------
Main functions signatures
------------------------------------------------------------
*/
corex_intg mainReader(corex_intg argc, corex_string* argv);
corex_intg mainScanner(corex_intg argc, corex_string* argv);
corex_intg mainParser(corex_intg argc, corex_string* argv);
corex_void printLogo();

#endif
