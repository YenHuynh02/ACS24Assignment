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

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

corex_void startParser() {
    corex_intg i = 0;
    for (i = 0; i < NUM_BNF_RULES; i++) {
        psData.parsHistogram[i] = 0;
    }
    lookahead = tokenizer();
    if (lookahead.code != SEOF_T) {
        program();
    }
    matchToken(SEOF_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

corex_void matchToken(corex_intg tokenCode, corex_intg tokenAttribute) {
    corex_intg matchFlag = 1;
    switch (lookahead.code) {
    case KW_T:
        if (lookahead.attribute.codeType != tokenAttribute)
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

corex_void printError() {
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
        printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
        break;
    case STR_T:
        printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case KW_T:
        printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
    default:
        printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
        numParserErrors++;
    }
}

corex_void program() {
    psData.parsHistogram[BNF_program]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
        break;
    case MNID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
            matchToken(MNID_T, NO_ATTR);
            matchToken(LBR_T, NO_ATTR);
            dataSession();
            codeSession();
            matchToken(RBR_T, NO_ATTR);
            break;
        }
        else {
            printError();
        }
        break;
    case SEOF_T:
        break; // Empty
    default:
        printError();
    }
    printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

corex_void comment() {
    psData.parsHistogram[BNF_comment]++;
    matchToken(CMT_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

corex_void dataSession() {
    psData.parsHistogram[BNF_dataSession]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
    default:
        matchToken(KW_T, KW_data);
        matchToken(LBR_T, NO_ATTR);
        optVarListDeclarations();
        matchToken(RBR_T, NO_ATTR);
        printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
    }
}

corex_void optVarListDeclarations() {
    psData.parsHistogram[BNF_optVarListDeclarations]++;
    switch (lookahead.code) {
    default:
        break; // Empty
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

corex_void codeSession() {
    psData.parsHistogram[BNF_codeSession]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
    default:
        matchToken(KW_T, KW_code);
        matchToken(LBR_T, NO_ATTR);
        optionalStatements();
        matchToken(RBR_T, NO_ATTR);
        printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
    }
}

corex_void optionalStatements() {
    psData.parsHistogram[BNF_optionalStatements]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
    case MNID_T:
        if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
            (strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
            statements();
            break;
        }
    default:
        break; // Empty
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

corex_void statements() {
    psData.parsHistogram[BNF_statements]++;
    statement();
    statementsPrime();
    printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

corex_void statementsPrime() {
    psData.parsHistogram[BNF_statementsPrime]++;
    switch (lookahead.code) {
    case MNID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
            statements();
            break;
        }
    default:
        break; // Empty
    }
}

corex_void statement() {
    psData.parsHistogram[BNF_statement]++;
    switch (lookahead.code) {
    case KW_T:
        switch (lookahead.attribute.codeType) {
        default:
            printError();
        }
        break;
    case MNID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
            outputStatement();
        }
        break;
    default:
        printError();
    }
    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

corex_void outputStatement() {
    psData.parsHistogram[BNF_outputStatement]++;
    matchToken(MNID_T, NO_ATTR);
    matchToken(LPR_T, NO_ATTR);
    outputVariableList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

corex_void outputVariableList() {
    psData.parsHistogram[BNF_outputVariableList]++;
    switch (lookahead.code) {
    case STR_T:
        matchToken(STR_T, NO_ATTR);
        break;
    default:
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

corex_void printBNFData(ParserData psData) {
    printf("Statistics:\n");
    printf("----------------------------------\n");
    int cont = 0;
    for (cont = 0; cont < NUM_BNF_RULES; cont++) {
        if (psData.parsHistogram[cont] > 0)
            printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
    }
    printf("----------------------------------\n");
}
