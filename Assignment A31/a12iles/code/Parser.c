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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TO_DO: This is the function to start the parser - check your program definition */

corex_void startParser() {
	/* TO_DO: Initialize Parser data */
	corex_intg i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TO_DO: This is the main code for match - check your definition */
corex_void matchToken(corex_intg tokenCode, corex_intg tokenAttribute) {
	corex_intg matchFlag = 1;
	switch (lookahead.code) {
	case FPL_T:
		if (tokenCode != FPL_T)
			matchFlag = 0;
		break;
	case INL_T:
		if (tokenCode != INL_T)
			matchFlag = 0;
		break;
	case STR_T:
		if (tokenCode != STR_T)
			matchFlag = 0;
		break;
	case VID_T:
		if (tokenCode != VID_T)
			matchFlag = 0;
		break;
	case LPR_T:
		if (tokenCode != LPR_T)
			matchFlag = 0;
		break;
	case RPR_T:
		if (tokenCode != RPR_T)
			matchFlag = 0;
		break;
	case LBR_T:
		if (tokenCode != LBR_T)
			matchFlag = 0;
		break;
	case RBR_T:
		if (tokenCode != RBR_T)
			matchFlag = 0;
		break;
	case ART_T:
		if (tokenCode != ART_T)
			matchFlag = 0;
		break;
	case SQ_T:
		if (tokenCode != SQ_T)
			matchFlag = 0;
			break;
	case DQ_T:
		if (tokenCode != DQ_T)
			matchFlag = 0;
			break;
	case KW_T:
		if (tokenCode != KW_T) 
			matchFlag = 0;
		break;
	case LG_T:
		if (tokenCode != LG_T)
			matchFlag = 0;
		break;
	case CSN_T:
		if (tokenCode != CSN_T) 
			matchFlag = 0;
		break;
	case LR_T:
		if (tokenCode != LR_T)
			matchFlag = 0;
		break;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		syncErrorHandler(tokenCode);
	}
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TO_DO: This is the function to handler error - adjust basically datatypes */
corex_void syncErrorHandler(corex_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TO_DO: This is the function to error printing - adjust basically datatypes */
corex_void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
		/*case VID_T:
			printf("VID_T\t\t%s\n", t.attribute.idLexeme);
			break;*/
	case STR_T:
		printf("STR_T\t\t%s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\t\t%s", readerGetContent(contentBuffer, t.attribute.contentString));
		break;
	case SQ_T:
		printf("SQ_T\n");
		break;
	case DQ_T:
		printf("DQ_T\n");
		break;
	case AO_T:
		printf("AO_T\t\t%s\n", readerGetContent(contentBuffer, t.attribute.contentString));
		break;
	case ART_T:
		printf("ART_T\t\t%s\n", readerGetContent(contentBuffer, t.attribute.contentString));
		break;
	case FPL_T:
		printf("FPL_T\t\t%.2f\n", t.attribute.floatValue);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case CSN_T:
		printf("CSN_T\n");
		break;
	case LR_T:
		printf("LR_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
		numParserErrors++;
	}
}

corex_void program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */
	while (lookahead.code != SEOF_T && lookahead.code != EOF) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
			break;
		case MNID_T:
			VarDeclarations();
			break;
		case VID_T:
		case AO_T:
		case FPL_T:
		case STR_T:
		case INL_T:
			dataDeclaration();
			break;
		case LPR_T:
		case RPR_T:
		case LBR_T:
		case RBR_T:
		case SQ_T:
		case DQ_T:
		case CSN_T:
		case LR_T:
			separator();
			break;
		case ART_T:
			arithmetic();
			break;
		case KW_T:
			keyword();
			break;
		case LG_T:
			conditionExpression();
			break;
		case SEOF_T:
			break;
		default:
			printError();
			lookahead = tokenizer();
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
corex_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/* TO_DO: Continue the development (all non-terminal functions) */
corex_void VarDeclarations() {
	psData.parsHistogram[BNF_VarDeclarations]++;
	matchToken(MNID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Variable parsed");
}

corex_void arithmetic() {
	switch (lookahead.code) {
		case ART_T:
			psData.parsHistogram[BNF_arithmetic]++;
			matchToken(ART_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Arithmetic parsed");
			break;

		default:
			break;
	}
}

corex_void keyword() {
	switch (lookahead.code) {
	case KW_T:
		psData.parsHistogram[BNF_keyword]++;
		matchToken(KW_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Keyword parsed");
		break;

	default:
		break;
	}
}

corex_void conditionExpression() {
	switch (lookahead.code) {
	case LG_T:
		psData.parsHistogram[BNF_conditionExpression]++;
		matchToken(LG_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Logical expression parsed");
		break;
	default:
		break;
	}
}

corex_void separator() {
		switch (lookahead.code) {
		case LPR_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(LPR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Left-paranthese parsed");
			break;
		case RPR_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(RPR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Right-paranthese parsed");
			break;
		case LBR_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(LBR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Left-brace parsed");
			break;
		case RBR_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(RBR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Right-brace parsed");
			break;
		case SQ_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(SQ_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Single quotation parsed");
			break;
		case DQ_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(DQ_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Double quotation parsed");
			break;
		case CSN_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(CSN_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Colon/ Semi-colon parsed");
			break;
		case LR_T:
			psData.parsHistogram[BNF_separator]++;
			matchToken(LR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Leftward/ Rightward parsed");
			break;
		default:
			break;
	}
}

corex_void dataDeclaration() {
		switch (lookahead.code) {
		case AO_T:
			psData.parsHistogram[BNF_assignmentOperator]++;
			matchToken(AO_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Assignment Operator parsed");
			break;
		case FPL_T:
			psData.parsHistogram[BNF_dataDeclaration]++;
			matchToken(FPL_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Floating-point Literals parsed");
			break;
		case INL_T:
			psData.parsHistogram[BNF_dataDeclaration]++;
			matchToken(INL_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Integer Literals parsed");
			break;
		case STR_T:
			psData.parsHistogram[BNF_dataDeclaration]++;
			matchToken(STR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": String Literals parsed");
			break;
		case MNID_T:
			VarDeclarations();
		default:
			break;
	}
}


/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
 /*
 corex_void printBNFData(ParserData psData) {
 }
 */
corex_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
