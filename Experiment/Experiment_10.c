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
char terminals[MAX_SYMBOLS];
char nonTerminals[MAX_SYMBOLS];
char parseTable[MAX_SYMBOLS][MAX_SYMBOLS][MAX_LEN];

int countTerminals = 0, countNonTerminals = 0;

/* ---------- Utility Functions ---------- */

int addToSet(char *set, char symbol) {
    if (symbol == '\0') return 0;
    for (int i = 0; set[i] != '\0'; i++)
        if (set[i] == symbol)
            return 0;
    int len = strlen(set);
    set[len] = symbol;
    set[len + 1] = '\0';
    return 1;
}

int getNTIndex(char nt) {
    for (int i = 0; i < countNonTerminals; i++)
        if (nonTerminals[i] == nt)
            return i;
    return -1;
}

int getTIndex(char t) {
    for (int i = 0; i < countTerminals; i++)
        if (terminals[i] == t)
            return i;
    return -1;
}

int isTerminal(char c) {
    return ((c >= 'a' && c <= 'z') || strchr("+-*/()=$#", c));
}

/* ---------- FIRST Computation ---------- */

void computeFirst(char symbol);

void computeFirst(char symbol) {
    if (symbol < 'A' || symbol > 'Z') return;
    int sIdx = symbol - 'A';
    if (firstComputed[sIdx]) return;

    firstComputed[sIdx] = true;

    for (int r = 0; r < numRules; r++) {
        if (grammar[r][0] != symbol) continue;
        const char *rhs = grammar[r] + 3;

        if (rhs[0] == '#' && rhs[1] == '\0') {
            addToSet(first[sIdx], '#');
            continue;
        }

        int nullable = 1;
        for (int j = 0; rhs[j] != '\0' && nullable; j++) {
            nullable = 0;
            char c = rhs[j];

            if (!(c >= 'A' && c <= 'Z')) {
                addToSet(first[sIdx], c);
            } else {
                computeFirst(c);
                int idx = c - 'A';
                for (int k = 0; first[idx][k] != '\0'; k++) {
                    if (first[idx][k] != '#')
                        addToSet(first[sIdx], first[idx][k]);
                }
                for (int k = 0; first[idx][k] != '\0'; k++) {
                    if (first[idx][k] == '#') {
                        nullable = 1;
                        break;
                    }
                }
            }
        }
        if (nullable)
            addToSet(first[sIdx], '#');
    }
}

/* ---------- FIRST of String ---------- */

void computeFirstOfString(const char *str, char *result) {
    result[0] = '\0';
    if (str == NULL || str[0] == '\0') return;

    int i = 0, nullable = 1;
    while (str[i] != '\0' && nullable) {
        nullable = 0;
        char c = str[i];
        if (!(c >= 'A' && c <= 'Z')) {
            addToSet(result, c);
        } else {
            computeFirst(c);
            int idx = c - 'A';
            for (int j = 0; first[idx][j] != '\0'; j++) {
                if (first[idx][j] != '#')
                    addToSet(result, first[idx][j]);
            }
            for (int j = 0; first[idx][j] != '\0'; j++) {
                if (first[idx][j] == '#') {
                    nullable = 1;
                    break;
                }
            }
        }
        if (!nullable) break;
        i++;
    }
    if (nullable) addToSet(result, '#');
}

/* ---------- FOLLOW Computation ---------- */

void computeFollow(char symbol) {
    if (symbol < 'A' || symbol > 'Z') return;
    int sIdx = symbol - 'A';
    if (followComputed[sIdx]) return;

    if (symbol == startSymbol)
        addToSet(follow[sIdx], '$');

    for (int i = 0; i < numRules; i++) {
        const char *rhs = grammar[i] + 3;
        int len = strlen(rhs);
        for (int j = 0; j < len; j++) {
            if (rhs[j] == symbol) {
                if (j + 1 < len) {
                    char next = rhs[j + 1];
                    if (isTerminal(next)) {
                        addToSet(follow[sIdx], next);
                    } else if (next >= 'A' && next <= 'Z') {
                        computeFirst(next);
                        int idx = next - 'A';
                        int hasEps = 0;
                        for (int k = 0; first[idx][k] != '\0'; k++) {
                            if (first[idx][k] == '#')
                                hasEps = 1;
                            else
                                addToSet(follow[sIdx], first[idx][k]);
                        }
                        if (hasEps && grammar[i][0] != symbol) {
                            computeFollow(grammar[i][0]);
                            int lhsIdx = grammar[i][0] - 'A';
                            for (int k = 0; follow[lhsIdx][k] != '\0'; k++)
                                addToSet(follow[sIdx], follow[lhsIdx][k]);
                        }
                    }
                } else if (grammar[i][0] != symbol) {
                    computeFollow(grammar[i][0]);
                    int lhsIdx = grammar[i][0] - 'A';
                    for (int k = 0; follow[lhsIdx][k] != '\0'; k++)
                        addToSet(follow[sIdx], follow[lhsIdx][k]);
                }
            }
        }
    }
    followComputed[sIdx] = true;
}

/* ---------- Parse Table Construction ---------- */

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
    for (int j = 0; j < countTerminals; j++)
        printf("%-10c", terminals[j]);
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

/* ---------- Grammar Parsing ---------- */

void addNonTerminal(char c) {
    if (c < 'A' || c > 'Z') return;
    for (int i = 0; i < countNonTerminals; i++)
        if (nonTerminals[i] == c)
            return;
    nonTerminals[countNonTerminals++] = c;
}

void addTerminal(char c) {
    if (!isTerminal(c)) return;
    for (int i = 0; i < countTerminals; i++)
        if (terminals[i] == c)
            return;
    terminals[countTerminals++] = c;
}

void parseProduction(char *input) {
    char lhs = input[0];
    addNonTerminal(lhs);

    int i = 3, start = 3;
    while (input[i] != '\0') {
        if (input[i] == '|' || input[i + 1] == '\0') {
            int end = (input[i] == '|') ? i : i + 1;
            grammar[numRules][0] = lhs;
            grammar[numRules][1] = '-';
            grammar[numRules][2] = '>';
            int k = 3;
            for (int j = start; j < end; j++) {
                grammar[numRules][k++] = input[j];
                if (isTerminal(input[j]))
                    addTerminal(input[j]);
                else
                    addNonTerminal(input[j]);
            }
            grammar[numRules][k] = '\0';
            numRules++;
            start = i + 1;
        }
        i++;
    }
}

/* ---------- Parsing Input ---------- */

void parseString(const char *input) {
    char stack[100];
    int top = 0;
    stack[top] = '$';
    stack[++top] = startSymbol;
    stack[top + 1] = '\0';

    int ip = 0;
    printf("\n%-20s%-20s%-20s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------------------\n");

    while (1) {
        printf("%-20s%-20s", stack, input + ip);

        char X = stack[top];
        char a = input[ip];

        if (X == '$' && a == '$') {
            printf("Accept\n");
            break;
        } else if (isTerminal(X) || X == '$') {
            if (X == a) {
                top--;
                ip++;
                stack[top + 1] = '\0';
                printf("Match %c\n", a);
            } else {
                printf("Error\n");
                break;
            }
        } else {
            int nIdx = getNTIndex(X);
            int tIdx = getTIndex(a);
            if (nIdx < 0 || tIdx < 0 || parseTable[nIdx][tIdx][0] == '\0') {
                printf("Error\n");
                break;
            }
            printf("%s\n", parseTable[nIdx][tIdx]);
            const char *rhs = parseTable[nIdx][tIdx] + 3;
            top--;
            for (int i = strlen(rhs) - 1; i >= 0; i--) {
                if (rhs[i] != '#')
                    stack[++top] = rhs[i];
            }
            stack[top + 1] = '\0';
        }
    }
}

/* ---------- MAIN ---------- */

int main() {
    int productionCount;
    char buffer[MAX_LEN];

    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    getchar();

    printf("Enter the productions (e.g., E->E+T|T):\n");
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

    for (int i = 0; i < countNonTerminals; i++)
        computeFirst(nonTerminals[i]);
    for (int i = 0; i < countNonTerminals; i++)
        computeFollow(nonTerminals[i]);

    printf("\nFIRST sets:\n");
    for (int i = 0; i < countNonTerminals; i++) {
        char nt = nonTerminals[i];
        printf("FIRST(%c) = { ", nt);
        for (int j = 0; first[nt - 'A'][j] != '\0'; j++)
            printf("%c ", first[nt - 'A'][j]);
        printf("}\n");
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < countNonTerminals; i++) {
        char nt = nonTerminals[i];
        printf("FOLLOW(%c) = { ", nt);
        for (int j = 0; follow[nt - 'A'][j] != '\0'; j++)
            printf("%c ", follow[nt - 'A'][j]);
        printf("}\n");
    }

    terminals[countTerminals++] = '$';
    printParseTable();

    printf("\nEnter input string to parse (e.g., id+id): ");
    char input[50];
    scanf("%s", input);
    strcat(input, "$");

    parseString(input);

    return 0;
}
