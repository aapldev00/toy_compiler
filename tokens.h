#ifndef TOKENS_H
#define TOKENS_H

#define MAX_LEXEME 512  // Tamaño máximo de un identificador o número

// Códigos de retorno según la Tabla 2 de la conferencia
typedef enum {
    TK_IF = 256,
    TK_THEN,
    TK_ELSE,
    TK_ID,
    TK_NUM,
    TK_LT,   // <
    TK_LTE,  // <=
    TK_EQ,   // =
    TK_NE,   // <>
    TK_GT,   // >
    TK_GTE,  // >=
    TK_ERROR,
    TK_EOF   // Fin de archivo
} TokenType;

typedef struct {
    int code;                  // El código del token (TK_ID, TK_NUM, etc.)
    char text[MAX_LEXEME];     // El lexema (tokenText)
    int line;                  // Opcional: línea donde se encontró (útil para errores)
} TInfo;

// Declaramos una variable global para que el Parser pueda leerla
extern TInfo tokenInfo;

#endif