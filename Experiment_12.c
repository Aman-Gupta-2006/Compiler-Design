#include <stdio.h>
#include <stdlib.h>

#define MAX 512
#define MAX_PROD 64
#define MAX_RHS 64

typedef struct {
    char lhs;
    char rhs[MAX_RHS];
    int rhs_len;
} Prod;

static Prod prods[MAX_PROD];
static int prod_count = 0;

static char stack_arr[MAX];
static int top = -1;

void push(char c) {
    if (top < MAX - 1)
        stack_arr[++top] = c;
}

char pop(void) {
    if (top >= 0)
        return stack_arr[top--];
    return '\0';
}

void print_state(const char *action, const char *input, int ipos) {
    char stack_str[MAX];
    int sidx = 0;

    for (int i = 0; i <= top; ++i)
        stack_str[sidx++] = stack_arr[i];
    stack_str[sidx] = '\0';

    printf("%-20s | %-20s | ", action, stack_str);
    for (int i = ipos; input[i] != '\0'; i++)
        putchar(input[i]);
    printf("\n");
}

int match_rhs_at_top(const char *rhs, int rhs_len) {
    if (rhs_len == 0) return 0;
    if (top + 1 < rhs_len) return 0;

    int start = top - rhs_len + 1;
    for (int i = 0; i < rhs_len; ++i)
        if (stack_arr[start + i] != rhs[i])
            return 0;
    return 1;
}

int try_reduce(void) {
    int best = -1, best_len = -1;
    for (int i = 0; i < prod_count; ++i) {
        int rlen = prods[i].rhs_len;
        if (rlen == 0) continue;
        if (match_rhs_at_top(prods[i].rhs, rlen)) {
            if (rlen > best_len) { best_len = rlen; best = i; }
        }
    }

    if (best >= 0) {
        for (int k = 0; k < prods[best].rhs_len; ++k)
            pop();
        push(prods[best].lhs);

        char action_str[64];
        int idx = 0;
        idx += sprintf(action_str, "Reduce: ");
        for (int k = 0; k < prods[best].rhs_len; ++k)
            idx += sprintf(action_str + idx, "%c", prods[best].rhs[k]);
        sprintf(action_str + idx, "->%c", prods[best].lhs);

        print_state(action_str, "", 0);
        return 1;
    }
    return 0;
}

int trim_newline(char *s) {
    int i = 0;
    while (s[i] != '\0') ++i;
    if (i > 0 && s[i - 1] == '\n') {
        s[i - 1] = '\0';
        --i;
    }
    return i;
}

void read_productions(void) {
    char line[MAX];
    printf("Enter productions (one per line), e.g. E->E+T. Blank line to finish:\n");

    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        trim_newline(line);

        int p = 0;
        while (line[p] == ' ' || line[p] == '\t') ++p;
        if (line[p] == '\0') break;

        char lhs = line[p];
        int arrow_pos = -1;
        for (int i = p + 1; line[i] != '\0'; ++i)
            if (line[i] == '-' && line[i + 1] == '>') {
                arrow_pos = i;
                break;
            }

        if (arrow_pos < 0) {
            printf("Invalid: %s\n", line + p);
            continue;
        }

        int r = arrow_pos + 2;
        while (line[r] == ' ' || line[r] == '\t') ++r;

        int ridx = 0;
        while (line[r] != '\0' && ridx < MAX_RHS - 1) {
            if (line[r] != ' ' && line[r] != '\t')
                prods[prod_count].rhs[ridx++] = line[r];
            ++r;
        }

        prods[prod_count].rhs[ridx] = '\0';
        prods[prod_count].rhs_len = ridx;
        prods[prod_count].lhs = lhs;
        ++prod_count;
    }

    if (prod_count == 0) {
        fprintf(stderr, "No productions entered.\n");
        exit(1);
    }
}

int main(void) {
    char input[MAX];
    read_productions();

    printf("Enter input expression (use single-char tokens, e.g. i+i*i):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;

    int len = trim_newline(input);
    input[len] = '$';
    input[len + 1] = '\0';

    top = -1;
    push('$');

    int ipos = 0;
    printf("\n%-20s | %-20s | %s\n", "Action", "Stack", "Input");
    printf("-------------------------------------------------------------------\n");
    print_state("Start", input, ipos);

    for (;;) {
        if (top == 1 && stack_arr[0] == '$' && stack_arr[1] == prods[0].lhs && input[ipos] == '$') {
            printf("-------------------------------------------------------------------\n");
            printf("\nInput accepted by the grammar (start symbol %c).\n", prods[0].lhs);
            return 0;
        }

        if (input[ipos] == '$') {
            while (try_reduce());
            if (top == 1 && stack_arr[0] == '$' && stack_arr[1] == prods[0].lhs && input[ipos] == '$') {
                printf("-------------------------------------------------------------------\n");
                printf("\n Input accepted by the grammar (start symbol %c).\n", prods[0].lhs);
                return 0;
            }
            printf("-------------------------------------------------------------------\n");
            printf("\nError: No valid shift or reduce possible.\n");
            return 1;
        }

        char cur = input[ipos];
        char act[32];
        sprintf(act, "Shift -> %c", cur);
        push(cur);
        ++ipos;
        print_state(act, input, ipos);

        while (try_reduce());
    }
}
