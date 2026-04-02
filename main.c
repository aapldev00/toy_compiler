#include <stdio.h>
#include "scanner.h"
#include "parser_cdr.h"

int main() {
    // Caso de prueba 1: Un IF simple con ELSE
    const char *source = "if x < 10 then y = 5.5E2 else z = 0";
    
    printf("Compilando código fuente: %s\n", source);
    printf("-------------------------------------------\n");

    // Inicializamos el Scanner con el código
    initScanner(source);

    // Ejecutamos el Parser
    parse();

    return 0;
}