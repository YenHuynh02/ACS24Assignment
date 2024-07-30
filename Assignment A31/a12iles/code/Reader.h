/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (corex) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */

/* TO_DO: BIT 3: END = End of buffer flag */
#define END 0x08	/* 2^3 or (0000.1000)_2 = (8)_10*/
/* TO_DO: BIT 2: REL = Rellocation memory flag */
#define REL 0x04 /* 2^2 or (0000.0100)_2 = (4)_10*/
/* TO_DO: BIT 1: EMP = Buffer empty flag */
#define EMP 0x02 /* 2^1 or (0000.0010)_2 = (2)_10 */
/* TO_DO: BIT 0: FUL = Buffer full flag */
#define FUL 0x01 /* 2^0 OR (0000.0001)_2 = (1)_10 */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (corex) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	corex_intg mark;			/* the offset to the mark position (in chars) */
	corex_intg read;			/* the offset to the get a char position (in chars) */
	corex_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	corex_string	content;			/* pointer to the beginning of character array (character buffer) */
	corex_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	corex_intg		increment;			/* character array increment factor */
	corex_intg		mode;				/* operational mode indicator */
	corex_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	corex_intg		histogram[NCHAR];	/* Statistics of chars */
	corex_intg		numReaderErrors;	/* Number of errors from Reader */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate(corex_intg, corex_intg, corex_intg);
BufferPointer	readerAddChar(BufferPointer const, corex_char);
corex_boln		readerClear(BufferPointer const);
corex_boln		readerFree(BufferPointer const);
corex_boln		readerIsFull(BufferPointer const);
corex_boln		readerIsEmpty(BufferPointer const);
corex_boln		readerSetMark(BufferPointer const, corex_intg);
corex_intg		readerPrint(BufferPointer const);
corex_intg		readerLoad(BufferPointer const, FILE* const);
corex_boln		readerRecover(BufferPointer const);
corex_boln		readerRetract(BufferPointer const);
corex_boln		readerRestore(BufferPointer const);
corex_void		readerChecksum(BufferPointer const);
/* Getters */
corex_char		readerGetChar(BufferPointer const);
corex_string	readerGetContent(BufferPointer const, corex_intg);
corex_intg		readerGetPosRead(BufferPointer const);
corex_intg		readerGetPosWrte(BufferPointer const);
corex_intg		readerGetPosMark(BufferPointer const);
corex_intg		readerGetSize(BufferPointer const);
corex_intg		readerGetInc(BufferPointer const);
corex_intg		readerGetMode(BufferPointer const);
corex_byte		readerGetFlags(BufferPointer const);
corex_void		readerPrintStat(BufferPointer const);
corex_intg		readerNumErrors(BufferPointer const);

#endif