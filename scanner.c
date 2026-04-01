#include <stdio.h>
#include <ctype.h>
#include "tokens.h"
#include <string.h>

#define MAX_LEXEME 512  // Tamaño máximo de un identificador o número

static char tokenText[MAX_LEXEME]; // El buffer físico
static int lex_idx = 0;            // El índice para saber dónde escribir el siguiente carácter del lexema actual.

// Prototipos de funciones para el escáner
char nextChar();
void retract(int chars);


static char *source_ptr; // Puntero a la fuente de entrada.
static char current_char; // Variable para almacenar el carácter actual.

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

int checkReserved() {
    if (strcmp(tokenText, "if") == 0) return TK_IF;
    if (strcmp(tokenText, "then") == 0) return TK_THEN;
    if (strcmp(tokenText, "else") == 0) return TK_ELSE;
    
    // Si no coincide con ninguna, es un identificador normal
    return TK_ID;
}

int nextToken() {
    int state = 0; // Estado inicial.

    clearBuffer();

    while (1) {
        current_char = nextChar(); // Lee el siguiente carácter.

        switch (state) {
            case 0:
                if (current_char == '\0') {
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
                        return TK_EQ; 
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
                            return TK_ERROR;
                        }
                }

            case 1:
                switch (current_char) {
                    case '>':
                        return TK_NE;
                    case '=':
                        return TK_LTE;
                    default:
                        retract(1);
                        return TK_LT;
                }

            case 6:
                switch (current_char) {
                    case '=':
                        return TK_GTE;
                    default:
                        retract(1);
                        return TK_GT;
                }

            case 9:
                if (isletter(current_char) || isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                } else {
                    // Estado 10: Fin del lexema
                    retract(1);
                    return checkReserved(); // Decidimos si es ID o palabra reservada
                }

            case 11:
                if (isdigit(current_char)) {
                    saveChar(current_char);
                    continue;
                }
                retract(1);
                return TK_NUM;

            default:
                return TK_ERROR;
        }
    }
}
