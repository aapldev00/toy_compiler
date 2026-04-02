#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.h"

// Prototipos públicos del escáner
void initScanner(const char *source);
int nextToken();

#endif