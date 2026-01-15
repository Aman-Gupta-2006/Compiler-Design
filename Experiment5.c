#include <stdio.h>

int stringLength(char s[]){
    int i = 0;
    while(s[i] != '\0'){
        i++;
    }
    return i;
}

void copyString(char src[], char dest[]){
    int i = 0;
    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void commonPrefix(char s1[], char s2[], char prefix[]){
    int i = 0;
    while(s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]){
        prefix[i] = s1[i];
        i++;
    }
    prefix[i] = '\0';
}

int main() {
    int n;
    printf("Enter the number of production rules: ");
    scanf("%d", &n);
    
    for(int p = 0; p < n; p++) {
        char input[100], nonTerminal;
        char prods[10][100], prefix[100], temp[100];
        int i, j, altCount = 0;
        
        printf("\nEnter the production rule: ");
        scanf("%s", input);
        
        nonTerminal = input[0];
        i = 3;
        
        while(1) {
            j = 0;
            while(input[i] != '|' && input[i] != '\0') {
                prods[altCount][j++] = input[i++];
            }
            prods[altCount][j] = '\0';
            altCount++;
            
            if(input[i] == '\0') break;
            else i++;
        }
        
        copyString(prods[0], prefix);
        int found = 0;
        
        for(i = 0; i < altCount; i++) {
            for(j = i + 1; j < altCount; j++) {
                commonPrefix(prods[i], prods[j], temp);
                if(temp[0] != '\0') {
                    copyString(temp, prefix);
                    found = 1;
                    break;
                }
            }
            if(found) break;
        }
        
        if(prefix[0] == '\0') {
            printf("No Left Factoring Needed. The production rules are as follows:\n");
            printf("%c->", nonTerminal);
            for(i = 0; i < altCount; i++) {
                printf("%s", prods[i]);
                if(i != altCount - 1)
                    printf("|");
            }
            printf("\n");
        } else {
            printf("Left Factoring Applied. The production rules are as follows:\n");
            printf("%c->", nonTerminal);
            printf("%s%c'", prefix, nonTerminal);
            
            int firstOther = 1;
            for(i = 0; i < altCount; i++) {
                int m = 0;
                while(prods[i][m] == prefix[m] && prefix[m] != '\0') m++;
                if(m == 0) {
                    if(firstOther) printf("|");
                    firstOther = 0;
                    printf("%s", prods[i]);
                }
            }
            printf("\n");
            
            printf("%c'->", nonTerminal);
            int first = 1;
            for(i = 0; i < altCount; i++) {
                int prelen = 0;
                while(prefix[prelen] != '\0') prelen++;
                int m = 0;
                while(prods[i][m] == prefix[m] && prefix[m] != '\0') m++;
                if(m != 0) {
                    if(!first) printf("|");
                    first = 0;
                    if(prods[i][prelen] == '\0')
                        printf("epsilon");
                    else
                        printf("%s", &prods[i][prelen]);
                }
            }
            printf("\n");
        }
    }
    return 0;
}
