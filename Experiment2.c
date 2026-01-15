#include <stdio.h>
#include <string.h>

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

int main() {
    char id[100];

    printf("Enter an identifier: ");
    scanf("%s", id);

    if (isIdentifier(id)) {
        printf("Valid Identifier\n");
        printf("Token Generated is <id, %s>\n", id);
    } else {
        printf("Invalid Identifier\n");
    }

    return 0;
}
