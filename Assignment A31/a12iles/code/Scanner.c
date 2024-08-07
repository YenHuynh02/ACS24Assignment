#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global objects - variables */
extern BufferPointer stringLiteralTable;
extern BufferPointer contentBuffer;
corex_intg line;
extern corex_intg errorNumber;

extern corex_intg stateType[NUM_STATES];
extern corex_string keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern corex_intg transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;
static BufferPointer sourceBuffer;
static int inStringLiteral = 0;  // To track if inside a string literal
static int varDetect = 0; // Track if an assignment operator was found

corex_intg startScanner(BufferPointer psc_buf) {
    for (corex_intg i = 0; i < NUM_TOKENS; i++)
        scData.scanHistogram[i] = 0;
    readerRecover(psc_buf);
    readerClear(stringLiteralTable);
    line = 1;
    sourceBuffer = psc_buf;
    return EXIT_SUCCESS;
}

Token tokenizer(corex_void) {
    Token currentToken = { 0 };
    Token previousToken = { 0 };
    Token afterToken = { 0 };
    corex_char c;
    corex_intg state = 0;
    corex_intg lexStart;
    corex_intg lexEnd;
    corex_intg lexLength;
    corex_intg i;

    while (1) {
        c = readerGetChar(sourceBuffer);

        switch (c) {
        case ' ':
        case '\t':
        case '\f':
            continue;
        case '\n':
            line++;
            continue;
        case ':':
        case ';':
            currentToken.code = CSN_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case '(':
            currentToken.code = LPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case ')':
            currentToken.code = RPR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case '{':
            currentToken.code = LBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case '}':
            currentToken.code = RBR_T;
            scData.scanHistogram[currentToken.code]++;
            return currentToken;
        case '\'':
            currentToken.code = SQ_T;
            scData.scanHistogram[currentToken.code]++;
            inStringLiteral = !inStringLiteral; // Toggle the inStringLiteral flag
            return currentToken;
        case '\"':
            currentToken.code = DQ_T;
            scData.scanHistogram[currentToken.code]++;
            inStringLiteral = !inStringLiteral; // Toggle the inStringLiteral flag
            return currentToken;
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
            currentToken.code = ART_T;
            lexemeBuffer = readerCreate(2, 0, MODE_FIXED); // Create a buffer for these operators
            if (!lexemeBuffer) {
                fprintf(stderr, "Scanner error: Can not create buffer\n");
                exit(1);
            }
            readerAddChar(lexemeBuffer, c);
            readerAddChar(lexemeBuffer, READER_TERMINATOR);
            currentToken.attribute.contentString = readerGetPosWrte(contentBuffer);
            readerAddChar(contentBuffer, c);
            readerAddChar(contentBuffer, CHARSEOF0); // Add string terminator
            scData.scanHistogram[currentToken.code]++;
            readerRestore(lexemeBuffer);
            return currentToken;
        
        case CHARSEOF0:
            currentToken.code = SEOF_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.seofType = SEOF_0;
            return currentToken;
        case CHARSEOF255:
            currentToken.code = SEOF_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.seofType = SEOF_255;
            return currentToken;
        default:
            if (c == '#') {
                lexStart = readerGetPosRead(sourceBuffer) - 1;
                readerSetMark(sourceBuffer, lexStart);
                while (c != '\n' && c != CHARSEOF0 && c != CHARSEOF255) {
                    c = readerGetChar(sourceBuffer);
                }
                readerRetract(sourceBuffer);
                lexEnd = readerGetPosRead(sourceBuffer);
                lexLength = lexEnd - lexStart;
                lexemeBuffer = readerCreate((corex_intg)lexLength + 2, 0, MODE_FIXED);
                if (!lexemeBuffer) {
                    fprintf(stderr, "Scanner error: Can not create buffer\n");
                    exit(1);
                }
                readerRestore(sourceBuffer);
                for (i = 0; i < lexLength; i++) {
                    readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
                }
                readerAddChar(lexemeBuffer, READER_TERMINATOR);
                currentToken = funcCMT(readerGetContent(lexemeBuffer, 0));
                readerRestore(lexemeBuffer);
                return currentToken;
            }
            else if (isalpha(c)) {
                lexStart = readerGetPosRead(sourceBuffer) - 1;
                readerSetMark(sourceBuffer, lexStart);
                while (isalnum(c)) {
                    c = readerGetChar(sourceBuffer);
                }
                readerRetract(sourceBuffer);
                lexEnd = readerGetPosRead(sourceBuffer);
                lexLength = lexEnd - lexStart;
                lexemeBuffer = readerCreate((corex_intg)lexLength + 2, 0, MODE_FIXED);
                if (!lexemeBuffer) {
                    fprintf(stderr, "Scanner error: Can not create buffer\n");
                    exit(1);
                }
                readerRestore(sourceBuffer);
                for (i = 0; i < lexLength; i++) {
                    readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
                }
                readerAddChar(lexemeBuffer, READER_TERMINATOR);

                if (inStringLiteral) {
                    currentToken = funcSL(readerGetContent(lexemeBuffer, 0)); // Handle string literals
                }
                else {
                    currentToken = funcKEY(readerGetContent(lexemeBuffer, 0)); // Handle keywords and IDs
                }
                readerRestore(lexemeBuffer);
                return currentToken;
            }
            else if (isdigit(c)) {
                lexStart = readerGetPosRead(sourceBuffer) - 1;
                readerSetMark(sourceBuffer, lexStart);
                corex_boln isFloat = COREX_FALSE;
                while (isdigit(c) || c == '.') {
                    if (c == '.') {
                        isFloat = COREX_TRUE;
                    }
                    c = readerGetChar(sourceBuffer);
                }
                readerRetract(sourceBuffer);
                lexEnd = readerGetPosRead(sourceBuffer);
                lexLength = lexEnd - lexStart;
                lexemeBuffer = readerCreate((corex_intg)lexLength + 2, 0, MODE_FIXED);
                if (!lexemeBuffer) {
                    fprintf(stderr, "Scanner error: Can not create buffer\n");
                    exit(1);
                }
                readerRestore(sourceBuffer);
                for (i = 0; i < lexLength; i++) {
                    readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
                }
                readerAddChar(lexemeBuffer, READER_TERMINATOR);
                if (isFloat) {
                    currentToken = funcFL(readerGetContent(lexemeBuffer, 0)); // Handle float literals
                }
                else {
                    currentToken = funcIL(readerGetContent(lexemeBuffer, 0)); // Handle integer literals
                }
                readerRestore(lexemeBuffer);
                return currentToken;
            }
            else {
                // Handle multi-character operators
                lexStart = readerGetPosRead(sourceBuffer) - 1;
                corex_char nextChar = readerGetChar(sourceBuffer);

                
                    if ((c == '<' && nextChar == '-') || (c == '-' && nextChar == '>')) {
                        /*if (currentToken.code = AO_T) {
                            previousToken.code = VID_T;
                            scData.scanHistogram[previousToken.code]++;    
                        }
                        varDetect = 1;*/
                        currentToken.code = AO_T;
                        lexemeBuffer = readerCreate(3, 0, MODE_FIXED); // Create a buffer for these operators
                        if (!lexemeBuffer) {
                            fprintf(stderr, "Scanner error: Can not create buffer\n");
                            exit(1);
                        }
                        readerAddChar(lexemeBuffer, c);
                        readerAddChar(lexemeBuffer, nextChar);
                        readerAddChar(lexemeBuffer, READER_TERMINATOR);
                        currentToken.attribute.contentString = readerGetPosWrte(contentBuffer);
                        readerAddChar(contentBuffer, c);
                        readerAddChar(contentBuffer, nextChar);
                        readerAddChar(contentBuffer, CHARSEOF0); // Add string terminator
                        scData.scanHistogram[currentToken.code]++;
                        readerRestore(lexemeBuffer);
                        return currentToken;
                    }

                    if (c == '<' || c == '>' || (c == '<' && nextChar == '=') || (c == '>' && nextChar == '=')) {
                        currentToken.code = LR_T;
                        lexemeBuffer = readerCreate(5, 0, MODE_FIXED); // Create a buffer for these operators
                        if (!lexemeBuffer) {
                            fprintf(stderr, "Scanner error: Can not create buffer\n");
                            exit(1);
                        }
                        readerAddChar(lexemeBuffer, c);
                        readerAddChar(lexemeBuffer, nextChar);
                        readerAddChar(lexemeBuffer, READER_TERMINATOR);
                        currentToken.attribute.contentString = readerGetPosWrte(contentBuffer);
                        readerAddChar(contentBuffer, c);
                        readerAddChar(contentBuffer, nextChar);
                        readerAddChar(contentBuffer, CHARSEOF0); // Add string terminator
                        scData.scanHistogram[currentToken.code]++;
                        readerRestore(lexemeBuffer);
                        return currentToken;
                    }

                    if ((c == '!' && nextChar == '=') || (c == '&' && nextChar == '&') || (c == '|' && nextChar == '|')) {
                        currentToken.code = LG_T;
                        lexemeBuffer = readerCreate(4, 0, MODE_FIXED); // Create a buffer for these operators
                        if (!lexemeBuffer) {
                            fprintf(stderr, "Scanner error: Can not create buffer\n");
                            exit(1);
                        }
                        readerAddChar(lexemeBuffer, c);
                        readerAddChar(lexemeBuffer, nextChar);
                        readerAddChar(lexemeBuffer, READER_TERMINATOR);
                        currentToken.attribute.contentString = readerGetPosWrte(contentBuffer);
                        readerAddChar(contentBuffer, c);
                        readerAddChar(contentBuffer, nextChar);
                        readerAddChar(contentBuffer, CHARSEOF0); // Add string terminator
                        scData.scanHistogram[currentToken.code]++;
                        readerRestore(lexemeBuffer);
                        return currentToken;
                    }

                    else {
                        readerRetract(sourceBuffer); // If not a multi-character operator, retract the nextChar
                        currentToken.code = ERR_T;
                        scData.scanHistogram[currentToken.code]++;
                        currentToken.attribute.errLexeme[0] = c;
                        currentToken.attribute.errLexeme[1] = CHARSEOF0;
                        return currentToken;
                    }
            }
        }
    }
}


corex_intg nextState(corex_intg state, corex_char c) {
    corex_intg col;
    corex_intg next;
    col = nextClass(c);
    next = transitionTable[state][col];
    if (DEBUG)
        printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
    assert(next != FS);
    if (DEBUG)
        if (next == FS) {
            printf("Scanner Error: Illegal state:\n");
            printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
            exit(1);
        }
    return next;
}

corex_intg nextClass(corex_char c) {
    corex_intg val = -1;
    if (c < 0 || c > 255) {
        return 8;
    }

    switch (c) {
    case CHRCOL2: /* _ */
        val = 2;
        break;
    case CHRCOL3: /* # */
        val = 3;
        break;
    case CHRCOL4: /* \n */
        val = 4;
        break;
    case CHRCOL5: /* . */
        val = 5;
        break;
    case CHRCOL6: /* ' */
        val = 6;
        break;
    case CHRCOL7: /* " */
        val = 7;
        break;
    case CHARSEOF0:
    case CHARSEOF255:
        val = 8;
        break;
    default:
        if (isalpha(c))
            val = 0;
        else if (isdigit(c))
            val = 1;
        else
            val = 8;
    }
    return val;
}

Token funcCMT(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_intg i = 0, len = (corex_intg)strlen(lexeme);
    currentToken.code = CMT_T;
    scData.scanHistogram[currentToken.code]++;

    // Add comment content to the stringLiteralTable
    currentToken.attribute.contentString = readerGetPosWrte(contentBuffer);
    for (i = 0; i < len; i++) {
        if (lexeme[i] == '\n') {
            line++;
        }
        readerAddChar(contentBuffer, lexeme[i]);
    }
    readerAddChar(contentBuffer, CHARSEOF0); // Add string terminator

    return currentToken;
}

Token funcFL(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_real tdouble;
    tdouble = (corex_real)atof(lexeme);
    currentToken.code = FPL_T;
    scData.scanHistogram[currentToken.code]++;
    currentToken.attribute.floatValue = tdouble;
    return currentToken;
}

Token funcIL(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_long tlong;
    if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
        currentToken = (*finalStateTable[ESNR])(lexeme); // Handle error if the integer is too long
    }
    else {
        tlong = atol(lexeme); // Convert string to long integer
        if (tlong >= 0 && tlong <= SHRT_MAX) { // Check if the integer is within a valid range
            currentToken.code = INL_T;
            scData.scanHistogram[currentToken.code]++;
            currentToken.attribute.intValue = (corex_intg)tlong; // Store the integer value in the token's attribute
        }
        else {
            currentToken = (*finalStateTable[ESNR])(lexeme); // Handle error if the integer is out of range
        }
    }
    return currentToken;
}


Token funcID(corex_string lexeme) {
    Token currentToken = { 0 };
    size_t length = strlen(lexeme);
    corex_char lastch = lexeme[length - 1];
    corex_intg isID = COREX_FALSE;
    /*currentToken.code = varDetect ? MNID_T : VID_T;*/
    currentToken.code = MNID_T;
    scData.scanHistogram[currentToken.code]++;
    strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
    return currentToken;
}

Token funcSL(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_intg i = 0, len = (corex_intg)strlen(lexeme);
    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

    for (i = 0; i < len; i++) {
        if (lexeme[i] == '\n') {
            line++;
        }
        readerAddChar(stringLiteralTable, lexeme[i]);
    }

    if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
        currentToken.code = RTE_T;
        strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
        errorNumber = RTE_CODE;
        return currentToken;
    }

    currentToken.code = STR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

Token funcKEY(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_intg kwindex = -1, j = 0;

    for (j = 0; j < KWT_SIZE; j++) {
        if (!strcmp(lexeme, keywordTable[j])) {
            kwindex = j;
            break;
        }
    }

    if (kwindex != -1) {
        currentToken.code = KW_T;
        currentToken.attribute.codeType = kwindex;
    }
    else {
        currentToken = funcID(lexeme);
    }

    return currentToken;
}

Token funcErr(corex_string lexeme) {
    Token currentToken = { 0 };
    corex_intg i = 0, len = (corex_intg)strlen(lexeme);
    if (len > ERR_LEN) {
        strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
        currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
        strcat(currentToken.attribute.errLexeme, "...");
    }
    else {
        strcpy(currentToken.attribute.errLexeme, lexeme);
    }
    for (i = 0; i < len; i++)
        if (lexeme[i] == '\n')
            line++;
    currentToken.code = ERR_T;
    scData.scanHistogram[currentToken.code]++;
    return currentToken;
}

corex_void printToken(Token t) {
    extern corex_string keywordTable[];
    switch (t.code) {
    case RTE_T:
        printf("RTE_T\t\t%s", t.attribute.errLexeme);
        if (errorNumber) {
            printf("%d", errorNumber);
            exit(errorNumber);
        }
        printf("\n");
        break;
    case ERR_T:
        printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
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
    default:
        printf("Scanner error: invalid token code: %d\n", t.code);
    }
}

corex_void printScannerData(ScannerData scData) {
    printf("Statistics:\n");
    printf("----------------------------------\n");
    int cont = 0;
    for (cont = 0; cont < NUM_TOKENS; cont++) {
        if (scData.scanHistogram[cont] > 0)
            printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
    }
    printf("----------------------------------\n");
}
