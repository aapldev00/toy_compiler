# Gramática de Expresiones Aritméticas

Esta gramática define expresiones aritméticas con suma y multiplicación, incluyendo paréntesis, identificadores y números.

## Producciones

- E → T E′
- E′ → + T E′ | ε
- T → F T′
- T′ → * F T′ | ε
- F → ( E ) | id | num

## Diagramas de Transición (Sintaxis Diagrams)

### E (Expresión)
```mermaid
graph TD
    E --> T
    T --> E_prime
```

### E′ (Prima de Expresión)
```mermaid
graph TD
    E_prime --> |+| PLUS[T]
    PLUS --> T
    T --> E_prime
    E_prime --> epsilon[ε]
```

### T (Término)
```mermaid
graph TD
    T --> F
    F --> T_prime
```

### T′ (Prima de Término)
```mermaid
graph TD
    T_prime --> |*| MULT[F]
    MULT --> F
    F --> T_prime
    T_prime --> epsilon[ε]
```

### F (Factor)
```mermaid
graph TD
    F --> |"("| LPAREN[E]
    LPAREN --> E
    E --> |")"| RPAREN
    F --> id
    F --> num
```