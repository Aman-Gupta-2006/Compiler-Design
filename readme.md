# Compiler Design Experiments

This repository contains C programs, lexical analyzer files, and supporting documents for various experiments in compiler design. Each experiment demonstrates a key concept in the construction of compilers.

## Directory Structure

- **Experiment1.c**: DFA for recognizing the keyword `while`.
- **Experiment2.c**: DFA for recognizing valid identifiers.
- **Experiment3.c**: Symbol table implementation.
- **Experiment4.c**: Symbol table with keyword recognition.
- **Experiment5.c**: Left factoring of grammar.
- **Experiment5_LeftRecursion.c**: Elimination of left recursion in grammar.
- **Experiment7and8.c**: Computation of FIRST and FOLLOW sets for a grammar.
- **Experiment_9.c**: Construction of predictive parsing table.
- **Experiment_10.c**: Implementation of predictive parser.
- **Experiment_11.c**: Three-address code generation for arithmetic expressions.
- **Experiment_12.c**: Stack-based code generation for arithmetic expressions.
- **lexical_analyzer.l**: Lex specification for a simple lexical analyzer.
- **lex.yy.c**: Generated C file from `lexical_analyzer.l` using Flex.
- **experimentX/**: (If present) Folders for additional files or outputs for each experiment.
- **Compiler_design.pdf**, **Compiler_design2.pdf**, **compiler_index.pdf**: Reference materials and notes.
- **Mission&Vision.pdf**: Department or course mission and vision document.

## How to Use

### Compile C Programs
Use `gcc` to compile any `.c` file. For example:
```sh
gcc Experiment1.c -o experiment1
./experiment1
```

### Lexical Analyzer
To use the lexical analyzer:
```sh
flex lexical_analyzer.l
gcc lex.yy.c -o lexical_analyzer -lfl
./lexical_analyzer
```

### Reference Documents
PDF and .pages files are for reference and can be opened with a PDF viewer or Apple Pages.

## Notes

- Each experiment demonstrates a specific concept in compiler design, such as lexical analysis, parsing, or code generation.
- The code is written in C and is intended for educational purposes.
- Some folders or files may contain outputs or additional resources for the experiments.

---

Feel free to explore each experiment and refer to the documentation for more details.
