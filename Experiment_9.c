#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RULES 20
#define MAX_LEN 20
#define MAX_SYMBOLS 26 

char grammar[MAX_RULES][MAX_LEN];
int numRules = 0;
char first[MAX_SYMBOLS][MAX_LEN];
char follow[MAX_SYMBOLS][MAX_LEN];
bool firstComputed[MAX_SYMBOLS];
bool followComputed[MAX_SYMBOLS];
char startSymbol;

int countTerminals = 0;
int countNonTerminals = 0;  

char terminals[MAX_SYMBOLS];
char nonTerminals[MAX_SYMBOLS];
char parseTable[MAX_SYMBOLS][MAX_SYMBOLS][MAX_LEN];

void computeFirstOfString(const char *str, char *result);

int getNTIndex(char nt) {
    for (int i = 0; i < countNonTerminals; i++) {
        if (nonTerminals[i] == nt)
            return i;
    }
    return -1;
}

int getTIndex(char t) {
    for (int i = 0; i < countTerminals; i++) {
        if (terminals[i] == t)
            return i;
    }
    return -1;
}


void printParseTable() {
    for (int i = 0; i < MAX_SYMBOLS; i++)
        for (int j = 0; j < MAX_SYMBOLS; j++)
            parseTable[i][j][0] = '\0';
            
    for (int i = 0; i < numRules; i++) {
        char lhs = grammar[i][0];
        const char *rhs = grammar[i] + 3;

        char firstSet[MAX_LEN] = "";
        computeFirstOfString(rhs, firstSet);

        for (int j = 0; firstSet[j] != '\0'; j++) {
            if (firstSet[j] != '#') {
                int ntIndex = getNTIndex(lhs);
                int tIndex = getTIndex(firstSet[j]);
                if (ntIndex != -1 && tIndex != -1)
                    strcpy(parseTable[ntIndex][tIndex], grammar[i]);
            }
        }

        for (int j = 0; firstSet[j] != '\0'; j++) {
            if (firstSet[j] == '#') {
                int ntIndex = getNTIndex(lhs);
                for (int k = 0; follow[lhs - 'A'][k] != '\0'; k++) {
                    int tIndex = getTIndex(follow[lhs - 'A'][k]);
                    if (ntIndex != -1 && tIndex != -1)
                        strcpy(parseTable[ntIndex][tIndex], grammar[i]);
                }
            }
        }
    }
    
    printf("\n\n=== LL(1) Parse Table ===\n\n");
    printf("%-10s", " ");
    for (int j = 0; j < countTerminals; j++) {
        printf("%-10c", terminals[j]);
    }
    printf("\n");

    for (int i = 0; i < countNonTerminals; i++) {
        printf("%-10c", nonTerminals[i]);
        for (int j = 0; j < countTerminals; j++) {
            if (strlen(parseTable[i][j]) > 0)
                printf("%-10s", parseTable[i][j]);
            else
                printf("%-10s", "-");
        }
        printf("\n");
    }
}


void getNonTerminals(char nt) {
    if(nt == '#') {
        return;
    }
    
    for(int i=0; i<26; i++){
        if(nonTerminals[i] == nt){
            return;
        }
    }
    
    nonTerminals[countNonTerminals] = nt;
    countNonTerminals++;
}

void getTerminals(char t){
    
    for(int i=0; i<26; i++){
        if(terminals[i] == t){
            return;
        }
    }
    
    terminals[countTerminals] = t;
    countTerminals++;
}

void addToSet(char *set, char symbol) {
    if (symbol == '\0') return;
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == symbol) return;
    }
    int len = strlen(set);
    set[len] = symbol;
    set[len + 1] = '\0';
}

void computeFirst(char symbol);


void computeFirst(char symbol) {
    int symIndex = symbol - 'A';
    if (firstComputed[symIndex]) return;
    firstComputed[symIndex] = true;

    for (int i = 0; i < numRules; i++) {
        if (grammar[i][0] != symbol) continue;

        const char *rhs = grammar[i] + 3;

        if (rhs[0] == '#' && rhs[1] == '\0') {
            addToSet(first[symIndex], '#');
            continue;
        }

        int nullable = 1;
        for (int j = 0; rhs[j] != '\0' && nullable; j++) {
            nullable = 0;

            if (rhs[j] >= 'a' && rhs[j] <= 'z') {
                addToSet(first[symIndex], rhs[j]);
            } else if (rhs[j] >= 'A' && rhs[j] <= 'Z') {
                computeFirst(rhs[j]);
                int index = rhs[j] - 'A';

                for (int k = 0; first[index][k] != '\0'; k++) {
                    if (first[index][k] != '#') {
                        addToSet(first[symIndex], first[index][k]);
                    }
                }

                for (int k = 0; first[index][k] != '\0'; k++) {
                    if (first[index][k] == '#') {
                        nullable = 1;
                        break;
                    }
                }
            }

            if (!nullable) break;
        }

        if (nullable) {
            addToSet(first[symIndex], '#');
        }
    }
}

void computeFirstOfString(const char *str, char *result) {
    if (str[0] == '\0') return;

    int i = 0;
    int nullable = 1;
    while (str[i] != '\0' && nullable) {
        nullable = 0;

        if (str[i] >= 'a' && str[i] <= 'z') {
            addToSet(result, str[i]);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            computeFirst(str[i]);
            int index = str[i] - 'A';
            for (int j = 0; first[index][j] != '\0'; j++) {
                if (first[index][j] != '#') {
                    addToSet(result, first[index][j]);
                }
            }

            for (int j = 0; first[index][j] != '\0'; j++) {
                if (first[index][j] == '#') {
                    nullable = 1;
                    break;
                }
            }
        }

        if (!nullable) return;
        i++;
    }

    if (nullable) {
        addToSet(result, '#');
    }
}


void computeFollow(char symbol) {
    if (followComputed[symbol - 'A']) return;
    followComputed[symbol - 'A'] = true;

    if (symbol == startSymbol) {
        addToSet(follow[symbol - 'A'], '$');
    }

    for (int i = 0; i < numRules; i++) {
        const char *rhs = grammar[i] + 3;
        int len = strlen(rhs);
        for (int j = 0; j < len; j++) {
            if (rhs[j] == symbol) {
                if (j + 1 < len) {
                    if (rhs[j + 1] >= 'a' && rhs[j + 1] <= 'z') {
                        addToSet(follow[symbol - 'A'], rhs[j + 1]);
                    } else if (rhs[j + 1] >= 'A' && rhs[j + 1] <= 'Z') {
                        computeFirst(rhs[j + 1]);
                        int idx = rhs[j + 1] - 'A';
                        for (int k = 0; first[idx][k] != '\0'; k++) {
                            if (first[idx][k] != '#') {
                                addToSet(follow[symbol - 'A'], first[idx][k]);
                            }
                        }
                    }
                }

                if (j + 1 >= len) {
                    if (grammar[i][0] != symbol) {
                        computeFollow(grammar[i][0]);
                        int lhsIdx = grammar[i][0] - 'A';
                        for (int k = 0; follow[lhsIdx][k] != '\0'; k++) {
                            addToSet(follow[symbol - 'A'], follow[lhsIdx][k]);
                        }
                    }
                }
            }
        }
    }
}

int isTerminal(char input){
    if((input >= 'a' && input <= 'z') || input == '+' || input == '-' || input == ')' || input == '(' || input == '*' || input == '$'){
        return 1;
    }
    
    return 0;
}

void parseProduction(char *input) {
   char lhs = input[0];
    int i = 0;
    
    while (input[i] != '\0') {
        if (input[i] == '-' && input[i + 1] == '>') {
            i += 2; 
            break;
        }
        i++;
    }

    int start = i;
    while (input[i] != '\0') {
        if (input[i] == '|' || input[i + 1] == '\0') {
            int end = (input[i] == '|') ? i : i + 1;

          
            grammar[numRules][0] = lhs;
            grammar[numRules][1] = '-';
            grammar[numRules][2] = '>';
            
            getNonTerminals(lhs);

    
            int k = 3; 
            for (int j = start; j < end; j++) {
                grammar[numRules][k++] = input[j];
                if(isTerminal(input[j])) {
                    getTerminals(input[j]);
                } else {
                    getNonTerminals(input[j]);
                }
            }
            grammar[numRules][k] = '\0';

            numRules++;

            start = i + 1; 
        }
        i++;
}
}

int main() {
    int productionCount;
    char buffer[MAX_LEN];

    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    getchar();  

    printf("Enter the productions (e.g., A->aB|c):\n");
    for (int i = 0; i < productionCount; i++) {
        printf("Rule %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; 
        parseProduction(buffer);
    }

    printf("Enter the start symbol: ");
    scanf(" %c", &startSymbol);

    for (int i = 0; i < MAX_SYMBOLS; i++) {
        first[i][0] = '\0';
        follow[i][0] = '\0';
        firstComputed[i] = false;
        followComputed[i] = false;
    }

    for (int i = 0; i < numRules; i++) {
        computeFirst(grammar[i][0]);
    }

    for (int i = 0; i < numRules; i++) {
        computeFollow(grammar[i][0]);
    }

    printf("\nFIRST sets:\n");
    for (int i = 0; i < numRules; i++) {
        char nt = grammar[i][0];
        if (i == 0 || grammar[i][0] != grammar[i - 1][0]) {
            printf("FIRST(%c) = { ", nt);
            for (int j = 0; first[nt - 'A'][j] != '\0'; j++) {
                printf("%c ", first[nt - 'A'][j]);
            }
            printf("}\n");
        }
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < numRules; i++) {
        char nt = grammar[i][0];
        if (i == 0 || grammar[i][0] != grammar[i - 1][0]) {
            printf("FOLLOW(%c) = { ", nt);
            for (int j = 0; follow[nt - 'A'][j] != '\0'; j++) {
                printf("%c ", follow[nt - 'A'][j]);
            }
            printf("}\n");
        }
    }
    
    terminals[countTerminals] = '$';
    countTerminals++;
    
    printParseTable();

    return 0;
}