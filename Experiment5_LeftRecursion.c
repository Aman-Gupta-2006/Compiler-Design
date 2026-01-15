#include <stdio.h>

int stringLength(char s[]) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

int main() {
    int n;
    printf("Enter number of production rules: ");
    scanf("%d", &n);

    for (int p = 0; p < n; p++) {
        char input[200], nonTerminal;
        char prods[10][100], alpha[10][100], beta[10][100];
        int altCount = 0, alphaCount = 0, betaCount = 0;

        printf("\nEnter production rule: ");
        scanf("%s", input);

        nonTerminal = input[0];

        int i = 3, j;
        while (1) {
            j = 0;
            while (input[i] != '|' && input[i] != '\0')
                prods[altCount][j++] = input[i++];
            prods[altCount][j] = '\0';
            altCount++;
            if (input[i] == '\0') break;
            else i++;
        }

        for (i = 0; i < altCount; i++) {
            if (prods[i][0] == nonTerminal) {
               
                int k = 0;
                for (j = 1; prods[i][j] != '\0'; j++)
                    alpha[alphaCount][k++] = prods[i][j];
                alpha[alphaCount][k] = '\0';
                alphaCount++;
            } else {
                
                int k = 0;
                for (j = 0; prods[i][j] != '\0'; j++)
                    beta[betaCount][k++] = prods[i][j];
                beta[betaCount][k] = '\0';
                betaCount++;
            }
        }

        if (alphaCount == 0) {
            printf("\nNo left recursion in %c\n", nonTerminal);
            printf("%c->", nonTerminal);
            for (i = 0; i < altCount; i++) {
                printf("%s", prods[i]);
                if (i != altCount - 1) printf("|");
            }
            printf("\n");
        } else {
            printf("\nLeft recursion eliminated for %c\n", nonTerminal);


            printf("%c->", nonTerminal);
            for (i = 0; i < betaCount; i++) {
                printf("%s%c'", beta[i], nonTerminal);
                if (i != betaCount - 1) printf("|");
            }
            printf("\n");
            
            printf("%c'->", nonTerminal);
            for (i = 0; i < alphaCount; i++) {
                printf("%s%c'", alpha[i], nonTerminal);
                if (i != alphaCount - 1) printf("|");
            }
            printf("|Epsilon\n");
        }
    }

    return 0;
}
