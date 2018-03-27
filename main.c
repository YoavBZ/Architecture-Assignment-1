#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Bignum
{
    long numOfQwords;
    long *value;
    char negative;
} Bignum;

typedef struct Stack
{
    int size;
    int maxSize;
    Bignum *stack[1024];
} Stack;

extern long mulByTen(long *value);
extern long addRecursively(long *value, long toAdd, long loopCounter);
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

Bignum *peak(Stack *s)
{
    return s->stack[s->size];
}

void freeStack(Stack *s)
{
    for (int i = 0; i < s->maxSize; i++)
        free(s->stack[i]);
    free(s);
}

void addQwordIfNeeded(Bignum *n, long next)
{
    if (next)
    {
        n->numOfQwords++;
        n->value = realloc(n->value, n->numOfQwords * 8);
        n->value[n->numOfQwords] = next;
    }
}

void mulByTenRecursively(Bignum *n)
{
    long next = 0;
    for (int i = 0; i < n->numOfQwords - 1; i++)
    {
        next = mulByTen(n->value[i]);
        if (next) // Checks whether the multiplication returned value from rdx
        {
            long loopCounter = n->numOfQwords - i;
            addRecursively(n->value[i + 1], next, loopCounter);
        }
    }
    next = mulByTen(n->value[n->numOfQwords]); // Multiplies the highest order qword
    addQwordIfNeeded(n, next);
}

int main(int argc, char **argv)
{
    // Initiate stack with a single Bignum
    Stack *s = malloc(sizeof(Stack));
    s->size = 0;
    Bignum *n = malloc(sizeof(Bignum));
    n->value = malloc(8);

    int c = 0;
    while ((c = fgetc(stdin)) != 'q' && c != EOF)
    {
        switch (c)
        {
        case '0' ... '9':
            mulByTenRecursively(n);
            long next = addRecursively(n->value, c - '0', n->numOfQwords);
            addQwordIfNeeded(n, next);
            break;

        case '_':
            n->negative = 1;
            break;

        case '\0' ... ' ':
            if (n->numOfQwords > 0)
            {
                push(s, n);
            }
            break;

        case '+':
            if (n->numOfQwords > 0)
                push(s, n);
            Bignum *n1 = pop(s);
            Bignum *n2 = pop(s);
            // printBignum(n1);
            // printBignum(n2);
            // Add n1 & n2
            break;

        case '-':
            if (n->numOfQwords > 0)
                push(s, n);
            n1 = pop(s);
            n2 = pop(s);
            printBignum(n1);
            printBignum(n2);
            break;

        case '*':
            if (n->numOfQwords > 0)
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