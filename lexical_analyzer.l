%{
#include <stdio.h>
%}

%%
[ \t\n]+                ; /* ignore whitespace */
"int"|"float"|"char"    { printf("KEYWORD(%s)\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("IDENTIFIER(%s)\n", yytext); }
[0-9]+                  { printf("NUMBER(%s)\n", yytext); }
"="                     { printf("ASSIGN(%s)\n", yytext); }
"+"                     { printf("PLUS(%s)\n", yytext); }
"-"                     { printf("MINUS(%s)\n", yytext); }
"*"                     { printf("MULT(%s)\n", yytext); }
"/"                     { printf("DIV(%s)\n", yytext); }
";"                     { printf("SEMICOLON(%s)\n", yytext); }
"("|")"|"["|"]"|"{"|"}" { printf("Parenthesis(%s)\n", yytext);}
.                       { printf("UNKNOWN(%s)\n", yytext); }
%%

int main(void) {
    yylex();
    return 0;
}

int yywrap(void) {
    return 1;
}

