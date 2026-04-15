# 🧠 Toy Compiler

**Laboratorio personal** en C. El objetivo principal es profundizar en las entrañas de los lenguajes de programación, comprendiendo cómo una secuencia de caracteres se transforma en lógica ejecutable.

![alt text](https://img.shields.io/badge/Language-C-blue.svg)


![alt text](https://img.shields.io/badge/Status-En%20Desarrollo-green.svg)


![alt text](https://img.shields.io/badge/Context-Aprendizaje%20Acad%C3%A9mico-orange.svg)
---
### 🏗️ Anatomía del Proyecto

| Componente | El Rol
| :--- | :--- |
| **Scanner** 🔍 | Un **DFA** que devora caracteres y los convierte en Tokens limpios. |
| **Parser** ⚖️ | Un **Analizador de Descenso Recursivo** que decide si el código tiene sentido. |
| **Tokens** 🦴 | El diccionario que define nuestro universo (IDs, Números, Operadores). |

---

### 📜 La Gramática Actual
Actualmente, el cerebro del compilador procesa tokens para expresiones aritméticas (+, *, (, ), identificadores, números, EOF).

### A futuro:
Implementar parser LL(1), AST y reconocimiento semántico.