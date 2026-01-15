#include <stdio.h>
#define MAX 100
char stack[MAX];
int top = -1;
int tempCount = 1;

void push(char c) {
    top++;
    stack[top] = c;
}

char pop() {
    if (top == -1)
        return '\0';
    char c = stack[top];
    top--;
    return c;
}

int precedence(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

void infixToPostfix(char infix[], char postfix[]) {
    int i = 0, k = 0;
    char c;

    while ((c = infix[i]) != '\0') {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            postfix[k] = c;
            k++;
        } else if (c == '(') {
            push(c);
        } else if (c == ')') {
            while (top != -1 && stack[top] != '(') {
                postfix[k] = pop();
                k++;
            }
            pop(); 
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (top != -1 && precedence(stack[top]) >= precedence(c)) {
                postfix[k] = pop();
                k++;
            }
            push(c);
        }
        i++;
    }

    while (top != -1) {
        postfix[k] = pop();
        k++;
    }

    postfix[k] = '\0';
}

void generateTAC(char postfix[]) {
    char operandStack[MAX][5];
    int opTop = -1;
    int i = 0, len = 0;
    char c;

    while ((c = postfix[i]) != '\0') {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            operandStack[++opTop][0] = c;
            operandStack[opTop][1] = '\0';
        } else {
            char op2[5], op1[5];
            int k;

            
            for (k = 0; operandStack[opTop][k] != '\0'; k++)
                op2[k] = operandStack[opTop][k];
            op2[k] = '\0';
            opTop--;

            
            for (k = 0; operandStack[opTop][k] != '\0'; k++)
                op1[k] = operandStack[opTop][k];
            op1[k] = '\0';
            opTop--;

            printf("t%d = %s %c %s\n", tempCount, op1, c, op2);

            operandStack[++opTop][0] = 't';
            operandStack[opTop][1] = (char)('0' + tempCount);
            operandStack[opTop][2] = '\0';

            tempCount++;
        }
        i++;
    }
}

int main() {
    char expr[MAX], postfix[MAX], rhs[MAX];
    char lhs;
    int i = 0, j = 0;

    printf("Enter an expression: ");
    scanf("%s", expr);

    lhs = expr[0];

    for (i = 2; expr[i] != '\0'; i++) {
        rhs[j] = expr[i];
        j++;
    }
    rhs[j] = '\0';

    infixToPostfix(rhs, postfix);

    printf("\nPostfix Expression: %s\n", postfix);
    printf("\n=== Three Address Code ===\n");
    generateTAC(postfix);

    printf("%c = t%d\n", lhs, tempCount - 1);

    return 0;
}
