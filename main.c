#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Bignum
{
    long numOfQwords;
    char *value;
    char negative;
} Bignum;

typedef struct Stack
{
    int size;
    int maxSize;
    Bignum stack[1024];
} Stack;

Stack s;

extern char mulByTen(char *value);
extern char addRecursively(char *value, short toAdd, long loopCounter);
// extern _add(Bignum a, Bignum b);
// extern _sub(Bignum a, Bignum b);
// extern _mul(Bignum a, Bignum b);
// extern _div(Bignum a, Bignum b);

void printBignum(Bignum *n)
{
    // for (int i = 0; i < n->number_of_digits; i++)
    // putchar(n->digits[i]);
    putchar('\n');
}

void push(Bignum n)
{
    if (s.stack[s.size].numOfQwords != 0)
    {
        // Free heap memory of unused Bignum
        free(s.stack[s.size].value);
    }
    s.stack[s.size++] = n;
    s.maxSize++;
}

Bignum pop()
{
    return s.stack[s.size--];
}

Bignum peak()
{
    return s.stack[s.size];
}

void freeStack()
{
    for (int i = 0; i < s.maxSize; i++)
    {
        free(s.stack[i].value);
    }
}

void addQwordIfNeeded(Bignum *n, char next)
{
    if (next)
    {
        n->numOfQwords++;
        n->value = realloc(n->value, n->numOfQwords);
        n->value[n->numOfQwords - 1] = next;
    }
}

void mulByTenRecursively(Bignum *n)
{
    char next = 0;
    next = mulByTen(&(n->value[n->numOfQwords - 1]));
    int i = n->numOfQwords - 2;
    addQwordIfNeeded(n, next);
    for (; i >= 0; i--)
    {
        // Adding dx to the next word, won't overflow
        next = mulByTen(&(n->value[i]));
        n->value[i + 1] += next;
    }
}

Bignum operate(Bignum n1, Bignum n2, char op)
{
    Bignum result;
    switch (op)
    {
    case '+':
        result.value[max(n1.numOfQwords, n2.numOfQwords) + 1];
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            result.negative = 1;
            result.value = _add(n1.value, n2.value);
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            result.value = _sub(n2.value, n1.value);
            if (n1.numOfQwords > n2.numOfQwords)
            {
                result.negative = 1;
            }
            else if (n1.numOfQwords < n2.numOfQwords)
            {
                result.negative = 0;
            }
            else
            {
                result.negative = findMax(n1, n2);
            }
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            result.value = _sub(n1.value, n2.value);
            if (n1.numOfQwords > n2.numOfQwords)
            {
                result.negative = 0;
            }
            else if (n1.numOfQwords < n2.numOfQwords)
            {
                result.negative = 1;
            }
            else
            {
                result.negative = findMax(n1, n2);
            }
        }
        else
        {
            // n1 and n2 are positive
            result.negative = 0;
            result.value = _add(n1.value, n2.value);
        }
        break;
    case '-':
        result.value[max(n1.numOfQwords, n2.numOfQwords) + 1];
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            result.value = _sub(n2.value, n1.value);
            if (n1.numOfQwords > n2.numOfQwords)
            {
                result.negative = 1;
            }
            else if (n1.numOfQwords < n2.numOfQwords)
            {
                result.negative = 0;
            }
            else
            {
                result.negative = findMax(n1, n2);
            }
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            result.negative = 1;
            result.value = _add(n1.value, n2.value);
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            result.negative = 0;
            result.value = _add(n1.value, n2.value);
        }
        else
        {
            // n1 and n2 are positive
            result.value = _sub(n1.value, n2.value);
            if (n1.numOfQwords > n2.numOfQwords)
            {
                result.negative = 0;
            }
            else if (n1.numOfQwords < n2.numOfQwords)
            {
                result.negative = 1;
            }
            else
            {
                result.negative = findMax(n1, n2);
            }
        }
        break;
    case '*':
        result.negative = n1.negative & n2.negative;
        result.value = _mul(n1.value, n2.value);
        break;
    case '/':
        result.negative = n1.negative & n2.negative;
        result.value = _div(n1.value, n2.value);
        break;
    }
    return result;
}

int main(int argc, char **argv)
{
    // Initiate stack with a single Bignum
    s.size = 0;
    Bignum n;
    n.numOfQwords = 1;
    n.value = malloc(1);
    char next = 0;
    int c = 0;
    while ((c = fgetc(stdin)) != 'q')
    {
        switch (c)
        {
        case '0' ... '9':
            mulByTenRecursively(&n);
            next = addRecursively(n.value, c - '0', n.numOfQwords - 1);
            addQwordIfNeeded(&n, next);
            break;

        case '_':
            n.negative = 1;
            break;

        case '\0' ... ' ':
            if (n.value[n.numOfQwords - 1] != 0) // Checks whether the MSB isn't 0
            {
                push(n);
                n.numOfQwords = 1;
                n.value = malloc(1);
                n.negative = 0;
            }
            break;

        case '+':
        case '-':
        case '*':
        case '/':
            if (n.value[n.numOfQwords - 1] != 0) // Checks whether the MSB isn't 0
            {
                push(n);
            }
            Bignum n2 = pop();
            Bignum n1 = pop();
            push(operate(n1, n2, c));
            break;

        case 'p':
            // printBignum(peak(s));
            break;
        }
    }
    freeStack();
    return 0;
}