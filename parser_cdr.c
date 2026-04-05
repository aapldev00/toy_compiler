#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "scanner.h"

// Variable global para el símbolo que estamos mirando (lookahead)
static int lookahead;

// Prototipos de las reglas de la gramática.
void E();
void E_prime();
void T();
void T_prime();
void F();

// Avanza y verifica tokens.
void match(int expectedToken) {
    if (lookahead == expectedToken) {
        lookahead = nextToken();
    } else {
        printf("Error Sintáctico: Se esperaba el token %d, se encontró %d ('%s')\n", 
                expectedToken, lookahead, tokenInfo.text);
        exit(1);
    }
}

// F → ( E ) | id | num
void F() {
    if (lookahead == TK_LPAREN) {
        match(TK_LPAREN);
        E();
        match(TK_RPAREN);
    } else if (lookahead == TK_ID) {
        match(TK_ID);
    } else if (lookahead == TK_NUM) {
        match(TK_NUM);
    } else {
        printf("Error: Se esperaba '(', identificador o número\n");
        exit(1);
    }
}

// T′ → * F T′ | ε
void T_prime() {
    if (lookahead == TK_MULT) {
        match(TK_MULT);
        F();
        T_prime();
    }
    // ε: no hacer nada
}

// T → F T′
void T() {
    F();
    T_prime();
}

// E′ → + T E′ | ε
void E_prime() {
    if (lookahead == TK_PLUS) {
        match(TK_PLUS);
        T();
        E_prime();
    }
    // ε: no hacer nada
}

// E → T E′
void E() {
    T();
    E_prime();
}

// Función principal del Parser
void parse() {
    lookahead = nextToken();  // Inicializamos el lookahead
    
    E();  // Iniciamos con el axioma E
    
    if (lookahead == TK_EOF) {
        printf("Análisis sintáctico exitoso.\n");
    } else {
        printf("Error: Datos inesperados después del final de la expresión.\n");
        exit(1);
    }
}