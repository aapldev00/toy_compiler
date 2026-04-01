#include <stdio.h>
#include <ctype.h>
#include "tokens.h"


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

int nextToken() {
    int state = 0; // Estado inicial.

    while(1) {
        current_char = nextChar(); // Lee el siguiente carácter.

        switch (state) {
            case 0:
                if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
                    continue; // Ignorar blancos (ws)
                }
                if (current_char == '<') { state = 1; continue; } 
                if (current_char == '=') return TK_EQ;             
                if (current_char == '>') { state = 6; continue; }

                if (isletter(current_char)) { state = 9; continue; }
                if (isdigit(current_char)) { state = 11; continue; }
                
                if (current_char == '\0') return TK_EOF;
                return TK_ERROR;
            case 1: 
                if (current_char == '>') { return TK_NE; }
                if (current_char == '=') { return TK_LTE;}
                retract(1);
                return TK_LT;
            case 6:
                if (current_char == '=') {return TK_GTE;}
                retract(1);
                return TK_GT;
            case 9:
                if (isletter(current_char) || isdigit(current_char)) {}
        }
}