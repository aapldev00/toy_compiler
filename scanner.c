#include <stdio.h>
#include <ctype.h>
#include "tokens.h"
#include <string.h>

static char tokenText[MAX_LEXEME]; // El buffer físico
static int lex_idx = 0;            // El índice para saber dónde escribir el siguiente carácter del lexema actual.
TInfo tokenInfo; // Variable global para almacenar la información del token actual, que el Parser puede leer después de llamar a nextToken().

// Prototipos de funciones para el escáner
char nextChar();
void retract(int chars);

static char *source_ptr; // Puntero a la fuente de entrada.


char nextChar() {
    return *source_ptr++; // Primero avanza y luego retorna el carácter actual, ++ tiene mayor prioridad que *.
}

void retract(int chars) {
    source_ptr -= chars; // Retrocede el puntero de la fuente por la cantidad de caracteres especificada.
}


int isletter(char c) { // El nombre realmente no es muy específico.   
    return isalpha(c) || c == '_'; // Letras y guiones bajos son considerados válidos como carácteres iniciales de identificadores.
}

// Limpia el buffer para empezar un nuevo token
void clearBuffer() {
    lex_idx = 0;
    tokenText[0] = '\0';
}

// Agrega un carácter al final del lexema actual
void saveChar(char c) {
    if (lex_idx < MAX_LEXEME - 1) {
        tokenText[lex_idx++] = c; // Primero se guarda y despues se incrementa el índice.
        tokenText[lex_idx] = '\0'; // Mantener siempre el fin de cadena
    }
}

void setToken(int code, const char *text) {
    tokenInfo.code = code;
    strncpy(tokenInfo.text, text, MAX_LEXEME - 1);
    tokenInfo.text[MAX_LEXEME - 1] = '\0';
}

int checkReserved() {
    if (strcmp(tokenText, "if") == 0) return TK_IF;
    if (strcmp(tokenText, "then") == 0) return TK_THEN;
    if (strcmp(tokenText, "else") == 0) return TK_ELSE;
    
    // Si no coincide con ninguna, es un identificador normal
    return TK_ID;
}

int nextToken() {
    int state = 0; // Estado inicial.
    char current_char; // Variable para almacenar el carácter actual.

    clearBuffer();

    while (1) {
        current_char = nextChar(); // Lee el siguiente carácter.

        switch (state) {
            case 0:
                if (current_char == '\0') {
                    setToken(TK_EOF, "EOF"); // O un string vacío ""
                    return TK_EOF;
                }

                if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
                    continue; // Ignorar blancos (ws)
                }

                switch (current_char) {
                    case '<':
                        state = 1;
                        continue;
                    case '=':
                        setToken(TK_EQ, "=");
                        return TK_EQ; // state 5
                    case '>':
                        state = 6;
                        continue;
                    default:
                        if (isletter(current_char)) {
                            saveChar(current_char);
                            state = 9;
                            continue;
                        } else if (isdigit(current_char)) {
                            saveChar(current_char);
                            state = 11;
                            continue;
                        } else {
                            clearBuffer();
                            saveChar(current_char);
                            setToken(TK_ERROR, tokenText);
                            return TK_ERROR;
                        }
                }

            case 1:
                switch (current_char) {
                    case '>':
                        setToken(TK_NE, "<>");
                        return TK_NE; // state 2
                    case '=':
                        setToken(TK_LTE, "<=");
                        return TK_LTE; // state 3
                    default:
                        retract(1);
                        setToken(TK_LT, "<");
                        return TK_LT; // state 4
                }

            case 6:
                switch (current_char) {
                    case '=':
                        setToken(TK_GTE, ">=");
                        return TK_GTE; // state 7
                    default:
                        retract(1);
                        setToken(TK_GT, ">");
                        return TK_GT; // state 8
                }

            case 9:
                if (isletter(current_char) || isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                } else {
                    retract(1);
                    int code = checkReserved();
                    setToken(code, tokenText);
                    return code; // state 10
                }

            case 11: // Parte entera
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                }
                switch (current_char) {
                    case '.':
                        saveChar(current_char);
                        state = 12;
                        break;
                    case 'E':
                    case 'e':
                        saveChar(current_char);
                        state = 14;
                        break;
                    default:
                        retract(1);
                        setToken(TK_NUM, tokenText);
                        return TK_NUM;
                }
                continue;

            case 12: // Tras el punto '.'
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    state = 13;
                    continue;
                }
                setToken(TK_ERROR, tokenText);
                return TK_ERROR; // Error: se esperaba dígito después de '.'

            
            case 13: // Parte decimal
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                }
                switch (current_char) {
                    case 'E':
                    case 'e':
                        saveChar(current_char);
                        state = 14;
                        break;
                    default:
                        retract(1);
                        setToken(TK_NUM, tokenText);
                        return TK_NUM;
                }
                continue;

            case 14: // Exponente 'E' o 'e'
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    state = 16;
                    continue;
                }
                switch (current_char) {
                    case '+':
                    case '-':
                        saveChar(current_char);
                        state = 15;
                        break;
                    default:
                        setToken(TK_ERROR, tokenText);
                        return TK_ERROR; // Error: "1.2E" sin signo ni número
                }
                continue;

            case 15: // Signo del exponente
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    state = 16;
                    continue;
                }
                setToken(TK_ERROR, tokenText);
                return TK_ERROR;

            case 16: // Dígitos del exponente
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                }
                retract(1);
                setToken(TK_NUM, tokenText);
                return TK_NUM;
                
            default:
                setToken(TK_ERROR, tokenText);
                return TK_ERROR;
                        
        }
    }
}
