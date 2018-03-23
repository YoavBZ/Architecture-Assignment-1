#include <stdio.h>
#include <stdlib.h>

typedef struct Bignum
{
    long number_of_digits;
    char *digits;
    char negative;
} Bignum;

typedef struct Stack
{
    int size;
    int maxSize;
    Bignum *stack[1024];
} Stack;

// extern _add(Bignum a, Bignum b);
// extern _sub(Bignum a, Bignum b);
// extern _mul(Bignum a, Bignum b);
// extern _div(Bignum a, Bignum b);

void printBignum(Bignum *n)
{
    for (int i = 0; i < n->number_of_digits; i++)
        putchar(n->digits[i]);
    putchar('\n');
}

void push(Stack *s, Bignum *n)
{
    s->stack[s->size++] = n;
    s->stack[s->size] = malloc(sizeof(Bignum));
    s->maxSize++;
}

Bignum *pop(Stack *s)
{
    return s->stack[s->size--];
}

Bignum *peak(Stack *s){
    return s->stack[s->size];
}

void freeStack(Stack *s)
{
    for (int i = 0; i < s->maxSize; i++)
        free(s->stack[i]);
    free(s);
}

int main(int argc, char **argv)
{
    // Initiate stack with a single Bignum
    Stack *s = malloc(sizeof(Stack));
    s->size = 0;
    Bignum *n = malloc(sizeof(Bignum));
    n->digits = malloc(1);

    int c = 0;
    while ((c = fgetc(stdin)) != 'q' && c != EOF)
    {
        switch (c)
        {
        case '0' ... '9':
            n->digits[n->number_of_digits++] = (char)c;
            n->digits = realloc(n->digits, n->number_of_digits + 1);
            break;

        case '_':
            n->negative = 1;
            break;

        case '\0' ... ' ':
            if (n->number_of_digits > 0)
                push(s, n);
            break;

        case '+':
            if (n->number_of_digits > 0)
                push(s, n);
            Bignum *n1 = pop(s);
            Bignum *n2 = pop(s);
            printBignum(n1);
            printBignum(n2);
            // Add n1 & n2
            break;

        case '-':
            if (n->number_of_digits > 0)
                push(s, n);
            n1 = pop(s);
            n2 = pop(s);
            printBignum(n1);
            printBignum(n2);
            break;

        case '*':
            if (n->number_of_digits > 0)
                push(s, n);
            n1 = pop(s);
            n2 = pop(s);
            printBignum(n1);
            printBignum(n2);
            // Mul n1 & n2
            break;

        case '/':
            if (n->number_of_digits > 0)
                push(s, n);
            n1 = pop(s);
            n2 = pop(s);
            printBignum(n1);
            printBignum(n2);
            // Div n1 & n2
            break;

        case 'p':
            printBignum(peak(s));
        }
    }
    freeStack(s);
    return 0;
}