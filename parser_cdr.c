#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "scanner.h"

// Variable global para el símbolo que estamos mirando (lookahead)
static int lookahead;

// Prototipos de las reglas de la gramática.
void if_stmt();
void else_part();
void condition();
void expression();
void statement();

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

// <expression> ::= ID | NUM
void expression() {
    if (lookahead == TK_ID) {
        match(TK_ID);
    } else if (lookahead == TK_NUM) {
        match(TK_NUM);
    } else {
        printf("Error: Se esperaba Identificador o Numero\n");
        exit(1);
    }
}

// <condition> ::= <expression> RELOP <expression>
void condition() {
    expression();
    
    // Verificamos si el lookahead es cualquiera de los 6 operadores relacionales
    if (lookahead >= TK_LT && lookahead <= TK_GTE) {
        match(lookahead);                                                            // Tenemos que consumir el operador.
    } else {
        printf("Error: Se esperaba un operador relacional (<, >, =, etc.)\n");
        exit(1);
    }
    
    expression(); 
}

// <statement> ::= ID = <expression> | <if_stmt>
void statement() {
    if (lookahead == TK_IF) {
        if_stmt();
    } else {
        match(TK_ID);
        match(TK_EQ);
        expression();
    }
}

// <else_part> ::= else <statement> | epsilon
void else_part() {
    if (lookahead == TK_ELSE) {
        match(TK_ELSE);
        statement();
    }
    // Si no hay ELSE, el CDR simplemente no hace nada y termina la regla con éxito.
}

// <if_stmt> ::= if <condition> then <statement> <else_part>
void if_stmt() {
    match(TK_IF);
    condition();
    match(TK_THEN);
    statement();
    else_part();
}

// Función principal del Parser
void parse() {
    lookahead = nextToken();                                    // Inicializamos el lookahead
    
    if_stmt();                                                  // Iniciamos con el axioma
    
    if (lookahead == TK_EOF) {
        printf("Analisis sintactico exitoso.\n");
    } else {
        printf("Error: Datos inesperados despues del final de la sentencia.\n");
    }
}