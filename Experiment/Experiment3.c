#include <stdio.h>
#include <string.h>
# define maxTokenCount 100

char symbol[maxTokenCount][100];
int symbolNumber[maxTokenCount];
int symbolCount = 0;

int isWhileKeyword(char keyword[]){
    int s = 0;
    int i = 0;
    int n = strlen(keyword);
    
    while(i < n){
        char ch = keyword[i];
        switch(s){
            case 0: 
                if(ch == 'w'){
                    s = 1;
                } else {
                    return 0;
                }
                
                break;
                
            case 1:
                if(ch == 'h'){
                    s = 2;
                } else {
                    return 0;
                }
                break;
            case 2: 
                if(ch == 'i'){
                    s = 3;
                } else {
                    return 0;
                }
                break;
            case 3:
                if(ch == 'l'){
                    s = 4;
                } else {
                    return 0;
                }
                break;
            case 4: 
                if(ch == 'e'){
                    s = 5;
                } else {
                    return 0;
                }
                break;
            case 5:
                return 0;
        }
        
        i++;
    }
    return s == 5;
}

int isIdentifier(char id[]) {
    int s = 0;
    int i = 0;
    int n = strlen(id);

    while (i < n) {
        char ch = id[i];
        switch (s) {
            case 0: 
                if ((ch >= 'a' && ch <= 'z') || 
                    (ch >= 'A' && ch <= 'Z') || 
                    ch == '_') {
                    s = 1;
                    break;
                } else {
                    return 0;
                }
            case 1:
                if ((ch >= 'a' && ch <= 'z') ||
                    (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9') ||
                    ch == '_') {
                    s = 1;
                    break;
                } else {
                    return 0;
                }
        }
        i++;
    }

    return s == 1;
}

int isArithmeticOp(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=');
}

int isAlreadyFound(char word[]){
    
    for(int i=0; i<100; i++){
        int isFound = 1;
        for(int j=0; j<strlen(word); j++){
            if(word[j] != symbol[i][j]){
                isFound = 0;
                break;
            }
        }
        
        if(isFound == 1){
            return symbolNumber[i];
        }
    }
    
    return -1;
}

int main() {
    char input[100];
    printf("Enter input: ");
    fgets(input, sizeof(input), stdin);

    int i = 0, tokenCount = 1;
    while (input[i] != '\0' && input[i] != '\n') {
        char word[100];
        int j = 0;
        
        while (input[i] == ' ') i++;

        if (isArithmeticOp(input[i])) {
            printf("<ARTH, %c>\n", input[i]);
            i++;
            continue;
        }

        while ((input[i] >= 'a' && input[i] <= 'z') ||
                    (input[i] >= 'A' && input[i] <= 'Z') ||
                    (input[i] >= '0' && input[i] <= '9') || input[i] == '_') {
            word[j++] = input[i++];
        }
        word[j] = '\0';

        if (strlen(word) > 0) {
            if (isWhileKeyword(word)) {
                printf("<%s, KEYWORD>\n", word);
            } else if (isIdentifier(word)) {

                int existingCount = isAlreadyFound(word);
                if(existingCount != -1){
                    printf("<%s, id%d>\n", word, existingCount);
                } else {
                    printf("<%s, id%d>\n", word, tokenCount);
                    strcpy(symbol[symbolCount],word);
                    symbolNumber[symbolCount] = tokenCount;
                    tokenCount++;
                    symbolCount++;
                }
            } 
        }
    }
    return 0;
}
