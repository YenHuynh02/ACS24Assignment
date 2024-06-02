/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
ECHO "=---------------------------------------="
ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S24)  |"
ECHO "=---------------------------------------="
ECHO "        @ @               @ @        "
ECHO "          @               @          "
ECHO "           @             @           "
ECHO "            @           @            "
ECHO "             @    C    @             "
ECHO "              @   O   @              "
ECHO "              @   R   @              "
ECHO "             @    E    @             "
ECHO "            @           @            "
ECHO "           @             @           "
ECHO "          @               @          "
ECHO "        @ @               @ @        "
ECHO "                                     "
ECHO "     Yen Huynh & Mohammed Muhsin     "
ECHO "[READER SCRIPT .........................]"             
#
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H
#define COMPILERS_H

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

 /* TO_DO: Adjust your language (cannot be "Sofia") */

 /* Language name */
#define STR_LANGNAME	"CoreX"

/* Logical constants - adapt for your language */
#define COREX_TRUE  1
#define COREX_FALSE 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/


/* TO_DO: Define your typedefs */
typedef char			corex_char;
typedef char* corex_string;
typedef int				corex_intg;
typedef float			corex_real;
typedef void			corex_void;

typedef unsigned char	corex_boln;
typedef unsigned char	corex_byte;

typedef long			corex_long;
typedef double			corex_doub;

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
(Code will be updated during next assignments)
------------------------------------------------------------
*/
corex_intg mainReader(corex_intg argc, corex_string* argv);

/*
TO_DO: Include later mainScaner (A22) and mainParser (A32)
*/
//corex_void mainScanner(corex_intg argc, corex_string* argv);
//
//corex_void mainParser(corex_intg argc, corex_string* argv);

corex_void printLogo();

#endif