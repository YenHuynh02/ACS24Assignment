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
#                                     "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 15

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	SEOF_T,		/*  9: Source end of files */
	RTE_T,		/* 10: Run-time error token */
	CMT_T,		/* 11: Comment token */
	LW_T,		/* 12: Leftward Operator*/
	RW_T,		/* 13: Rightward Operator*/
	EQ_T		/* 14: Equal operator*/
};

/* TO_DO: Define the list of keywords */
static corex_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"SEOF_T",
	"RTE_T",
	"CMT_T",
	"LW_T",
	"RW_T",	
	"EQ_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	corex_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	corex_intg intValue;				/* integer literal attribute (value) */
	corex_intg keywordIndex;			/* keyword index in the keyword table */
	corex_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	corex_real floatValue;				/* floating-point literal attribute (value) */
	corex_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	corex_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	corex_byte flags;			/* Flags information */
	union {
		corex_intg intValue;				/* Integer value */
		corex_real floatValue;			/* Float value */
		corex_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	corex_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	corex_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

 /* TO_DO: Define lexeme FIXED classes */
 /* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '&'
#define CHRCOL4 '\''
#define CHRCOL6 '#'
#define CHRCOL7 '\n'

/* These constants will be used on VID / MID function */
#define MNID_SUF '&'
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		11
#define CHAR_CLASSES	9

/* TO_DO: Transition table - type of states defined in separate table */
static corex_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*    [A-z],[0-9],    _,    #,    \n,    .,    ',    ",   +,-,*,/, other
		   L(0), D(1), U(2), H(3), N(4), P(5), Q(6), R(7),  O(8) */
		{     1, ESNR, ESNR,    1,    2, ESNR,    9, ESNR,   7},	// S0: NOAS
		{     1,    1,    1,    1,    2,    1,    1,    1,   1},	// S1: NOAS
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS},	// S2: ASNR (SLC)
		{     3,    3,    3,    5,    4,    3,    3,    3,   3},	// S3: NOAS
		{     4,    4,    4,    4,    4,    4,    4,    4,   4},	// S4: NOAS
		{     5,    5,    5,    5,    5,    5,    5,    5,   5},	// S5: NOAS
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS},	// S6: ASNR (MLC)
		{     7,    7,    7,    7,    7,    7,    8,    7,   7},	// S7: NOAS
		{     8,    8,    8,    8,    8,    8,    8,    8,   8},	// S8: ASNR (VID | MID | KEY)
		{     9,    9,    9,    9,    9,    9,    9,   10,   9},	// S9: NOAS
		{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS}		// S10: ASNR (SL)
};

/* TO_DO: Transition table - type of states defined in separate table */
//static corex_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
//	/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
//		   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */
//		{     1, ESNR, ESNR, ESNR,    4, ESWR,	  6, ESNR},	// S0: NOAS
//		{     1,    1,    1,    2,	  3,    3,   3,    3},	// S1: NOAS
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S2: ASNR (MVID)
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S3: ASWR (KEY)
//		{     4,    4,    4,    4,    5, ESWR,	  4,    4},	// S4: NOAS
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S5: ASNR (SL)
//		{     6,    6,    6,    6,    6, ESWR,	  7,    6},	// S6: NOAS
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S7: ASNR (COM)
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S8: ASNR (ES)
//		{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS}  // S9: ASWR (ER)
//};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static corex_intg stateType[NUM_STATES] = {
	//NOFS, /* 00 */
	//NOFS, /* 01 */
	//FSNR, /* 02 (MID) - Methods */
	//FSWR, /* 03 (KEY) */
	//NOFS, /* 04 */
	//FSNR, /* 05 (SL) */
	//NOFS, /* 06 */
	//FSNR, /* 07 (COM) */
	//FSNR, /* 08 (Err1 - no retract) */
	//FSWR  /* 09 (Err2 - retract) */
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (SLC) - Single Line Comment */
	NOFS, /* 03 */
	NOFS, /* 04 */
	NOFS, /* 05 */
	FSNR, /* 06 (MLC) - Multi Line Comment */
	NOFS, /* 07 */
	FSNR, /* 08 (VID | MID | KEY) */
	NOFS, /* 09 */
	FSNR  /* 10 (SL) - String Literal */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
corex_intg			startScanner(BufferPointer psc_buf);
static corex_intg	nextClass(corex_char c);					/* character class function */
static corex_intg	nextState(corex_intg, corex_char);		/* state machine function */
corex_void			printScannerData(ScannerData scData);
Token				tokenizer(corex_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(corex_string lexeme);

/* Declare accepting states functions */
Token funcSL(corex_string lexeme);
Token funcIL(corex_string lexeme);
Token funcID(corex_string lexeme);
Token funcCMT(corex_string lexeme);
Token funcKEY(corex_string lexeme);
Token funcErr(corex_string lexeme);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcCMT,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static corex_string keywordTable[KWT_SIZE] = {
	"data",		/* KW00 */
	"code",		/* KW01 */
	"int",		/* KW02 */
	"real",		/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do"		/* KW09 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

 /* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	corex_char indentationCharType;
	corex_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
corex_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
