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
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern BufferPointer	contentBuffer;
extern corex_intg		line;
extern Token			tokenizer(corex_void);
extern corex_string		keywordTable[KWT_SIZE];
static corex_intg		syntaxErrorNumber = 0;


/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

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

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 17

/* Parser */
typedef struct parserData {
	corex_intg parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
corex_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
corex_void startParser();
corex_void matchToken(corex_intg, corex_intg);
corex_void syncErrorHandler(corex_intg);
corex_void printError();
corex_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_comment,									/*  1 */
	BNF_optPackage,									/*  2 */
	BNF_VarDeclarations,							/*  3 */
	BNF_arithmetic,									/*  4 */
	BNF_iteration,									/*  5 */
	BNF_keyword,									/*  6 */
	BNF_stringExpression,							/*  7 */
	BNF_conditionExpression,						/*  8 */
	BNF_relationalExpression,						/*  9 */
	BNF_dataDeclaration,							/* 10 */
	BNF_program,									/* 11 */
	BNF_EOF,										/* 12 */
	BNF_assignmentOperator,							/* 13 */
	BNF_separator									/* 14 */
};


/* TO_DO: Define the list of keywords */
static corex_string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",										
	"BNF_comment",									
	"BNF_optPackage",
	"BNF_VarDeclarations",											
	"BNF_arithmetic",							
	"BNF_iteration",
	"BNF_keyword",
	"BNF_stringExpression",
	"BNF_conditionExpression",
	"BNF_relationalExpression",
	"BNF_dataDeclaration",
	"BNF_program",
	"BNF_EOF",
	"BNF_assignmentOperator",
	"BNF_separator"
};

/* TO_DO: Place ALL non-terminal function declarations */
corex_void comment();
corex_void program();
corex_void VarDeclarations();
corex_void arithmetic();
corex_void keyword();
corex_void conditionExpression();
corex_void separator();
corex_void dataDeclaration();

#endif
