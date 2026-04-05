#ifndef TOKENS_H
#define TOKENS_H

#define MAX_LEXEME 512  // Tamaño máximo de un identificador o número

// Códigos de retorno para tokens de expresiones aritméticas
typedef enum {
    TK_PLUS = 256,   // +
    TK_MULT,         // *
    TK_LPAREN,       // (
    TK_RPAREN,       // )
    TK_ID,           // identificador
    TK_NUM,          // número
    TK_EOF,          // Fin de archivo
    TK_ERROR         // Error léxico
} TokenType;

typedef struct {
    int code;                  // El código del token (TK_ID, TK_NUM, etc.)
    char text[MAX_LEXEME];     // El lexema (tokenText)
    int line;                  // Opcional: línea donde se encontró (útil para errores)
} TInfo;

// Declaramos una variable global para que el Parser pueda leerla
extern TInfo tokenInfo;

#endif