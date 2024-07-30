/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferPointer stringLiteralTable;
extern corex_intg line;
extern Token tokenizer(corex_void);
extern corex_string keywordTable[KWT_SIZE];
static corex_intg syntaxErrorNumber = 0;

#define LANG_WRTE "print&"
#define LANG_READ "input&"
#define LANG_MAIN "main&"

/* Constants */
enum KEYWORDS {
    NO_ATTR = -1,
    KW_if,
    KW_else,
    KW_repeat,
    KW_while,
    KW_function,
    KW_for,
    KW_in,
    KW_next,
    KW_break,
    KW_TRUE,
    KW_FALSE,
    KW_NULL,
    KW_Inf,
    KW_print
};

/* Define the number of BNF rules */
#define NUM_BNF_RULES 12

/* Parser */
typedef struct parserData {
    corex_intg parsHistogram[NUM_BNF_RULES]; /* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
corex_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
corex_void startParser();
corex_void printBNFData(ParserData psData);
corex_void matchToken(corex_intg, corex_intg);
corex_void syncErrorHandler(corex_intg);
corex_void printError();

/* List of BNF statements */
enum BNF_RULES {
    BNF_error, /*  0: Error token */
    BNF_codeSession, /*  1 */
    BNF_comment, /*  2 */
    BNF_dataSession, /*  3 */
    BNF_optVarListDeclarations, /*  4 */
    BNF_optionalStatements, /*  5 */
    BNF_outputStatement, /*  6 */
    BNF_outputVariableList, /*  7 */
    BNF_program, /*  8 */
    BNF_statement, /*  9 */
    BNF_statements, /* 10 */
    BNF_statementsPrime /* 11 */
};

/* Define the list of keywords */
static corex_string BNFStrTable[NUM_BNF_RULES] = {
    "BNF_error",
    "BNF_codeSession",
    "BNF_comment",
    "BNF_dataSession",
    "BNF_optVarListDeclarations",
    "BNF_optionalStatements",
    "BNF_outputStatement",
    "BNF_outputVariableList",
    "BNF_program",
    "BNF_statement",
    "BNF_statements",
    "BNF_statementsPrime"
};

/* Place ALL non-terminal function declarations */
corex_void codeSession();
corex_void comment();
corex_void dataSession();
corex_void optVarListDeclarations();
corex_void optionalStatements();
corex_void outputStatement();
corex_void outputVariableList();
corex_void program();
corex_void statement();
corex_void statements();
corex_void statementsPrime();

#endif
