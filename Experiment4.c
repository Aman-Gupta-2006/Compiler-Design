#include <stdio.h>
#include <string.h>
#define maxTokenCount 100

char symbol[maxTokenCount][100];
int symbolNumber[maxTokenCount];
int symbolCount = 0;

int isWhileKeyword(char keyword[]) {
    int s = 0, i = 0, n = strlen(keyword);
    while (i < n) {
        char ch = keyword[i];
        switch (s) {
            case 0: if (ch == 'w') s = 1; else return 0; break;
            case 1: if (ch == 'h') s = 2; else return 0; break;
            case 2: if (ch == 'i') s = 3; else return 0; break;
            case 3: if (ch == 'l') s = 4; else return 0; break;
            case 4: if (ch == 'e') s = 5; else return 0; break;
            case 5: return 0;
        }
        i++;
    }
    return s == 5;
}

int isNumber(char constant[]) {
    int i = 0, dotCount = 0;
    int n = strlen(constant);
    if (n == 0) return 0;

    if (!(constant[i] >= '0' && constant[i] <= '9'))
        return 0;

    for (i = 0; i < n; i++) {
        char ch = constant[i];
        if (ch == '.') {
            dotCount++;
            if (dotCount > 1) return 0; 
        } else if (!(ch >= '0' && ch <= '9')) {
            return 0; 
        }
    }
    return 1;
}

int isIdentifier(char id[]) {
    int i = 0, n = strlen(id);
    if (n == 0) return 0;

    if (!((id[0] >= 'a' && id[0] <= 'z') || 
          (id[0] >= 'A' && id[0] <= 'Z') || 
           id[0] == '_')) {
        return 0;
    }

    for (i = 1; i < n; i++) {
        char ch = id[i];
        if (!((ch >= 'a' && ch <= 'z') || 
              (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9') ||
              ch == '_')) {
            return 0;
        }
    }
    return 1;
}

int isArithmeticOp(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=');
}

int isRelationalOp(char ch) {
    return (ch == '<' || ch == '>' || ch == '!' || ch == '=');
}

int isParenthesis(char ch) {
    return (ch == '(' || ch == ')');
}

int isAlreadyFound(char word[]) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(word, symbol[i]) == 0)
            return symbolNumber[i];
    }
    return -1;
}

int main() {
    char input[200];
    printf("Enter input: ");
    fgets(input, sizeof(input), stdin);

    int i = 0, tokenCount = 1;
    while (input[i] != '\0' && input[i] != '\n') {
        char word[100];
        int j = 0;

        while (input[i] == ' ') {
            printf("<WS,>\n");
            i++;
        }

        if (isArithmeticOp(input[i])) {
            printf("<ARTH, %c>\n", input[i]);
            i++;
            continue;
        }

        if (isRelationalOp(input[i])) {
            if (input[i + 1] == '=') {
                printf("<REL, %c=%c>\n", input[i], input[i + 1]);
                i += 2;
            } else {
                printf("<REL, %c>\n", input[i]);
                i++;
            }
            continue;
        }

        if (isParenthesis(input[i])) {
            printf("<PAR, %c>\n", input[i]);
            i++;
            continue;
        }


        while ((input[i] >= 'a' && input[i] <= 'z') ||
               (input[i] >= 'A' && input[i] <= 'Z') ||
               (input[i] >= '0' && input[i] <= '9') ||
               input[i] == '_') {
            word[j++] = input[i++];
        }
        word[j] = '\0';

        if (strlen(word) > 0) {
            if (isWhileKeyword(word)) {
                printf("<%s, KEYWORD>\n", word);
            } else if (isIdentifier(word)) {
                int existingCount = isAlreadyFound(word);
                if (existingCount != -1) {
                    printf("<%s, id%d>\n", word, existingCount);
                } else {
                    printf("<%s, id%d>\n", word, tokenCount);
                    strcpy(symbol[symbolCount], word);
                    symbolNumber[symbolCount] = tokenCount;
                    tokenCount++;
                    symbolCount++;
                }
            } else if (isNumber(word)) {
                printf("<NUM, %s>\n", word);
            } else {
                printf("<INVALID, %s>\n", word);
            }
        }
    }

    return 0;
}

