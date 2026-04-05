#include <stdio.h>
#include "scanner.h"
#include "parser_cdr.h"

int main() {
    // Caso de prueba: Expresión aritmética
    const char *source = "a + b * (c + d)";
    
    printf("Analizando expresión: %s\n", source);
    printf("-------------------------------------------\n");

    // Inicializamos el Scanner con la expresión.
    initScanner(source);

    // Ejecutamos el Parser.
    parse();

    return 0;
}