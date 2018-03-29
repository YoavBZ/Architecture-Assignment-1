#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Bignum
{
    char *value;
    long numOfQwords;
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

// Return 1 if |n1|>|n2|, -1 if |n1|<|n2|, and 0 if there're equal
char compareAbs(Bignum n1, Bignum n2)
{
    if (n1.numOfQwords > n2.numOfQwords)
    {
        return 1;
    }
    else if (n1.numOfQwords < n2.numOfQwords)
    {
        return -1;
    }
    for (int i = n1.numOfQwords - 1; i >= 0; i--)
    {
        if (n1.value[i] != n2.value[i])
            return n1.value[i] > n2.value[i] ? 1 : -1;
    }
    return 0;
}

void mulByTenRecursively(Bignum *n)
{
    char next = 0;
    next = mulByTen(&(n->value[n->numOfQwords - 1]));
    int i = n->numOfQwords - 2;
    addQwordIfNeeded(n, next);
    for (; i >= 0; i--)
    {
        // Adding ah to the next word, won't overflow
        n->value[i + 1] += mulByTen(&(n->value[i]));
    }
}

Bignum operate(Bignum n1, Bignum n2, char op)
{
    Bignum result;
    switch (op)
    {
    case '+':
        result.value[max(n1.numOfQwords, n2.numOfQwords)];
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            result.negative = 1;
            _add(n1, n2, result, maxWords(n1, n2));
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            _sub(n2, n1, result);
            if (compareAbs(n1, n2) == 1)
            {
                result.negative = 1;
            }
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            _sub(n1, n2, result);
            if (compareAbs(n1, n2) == -1)
            {
                result.negative = 1;
            }
        }
        else
        {
            // n1 and n2 are positive
            result.negative = 0;
            _add(n1, n2, result);
        }
        break;
    case '-':
        result.value[max(n1.numOfQwords, n2.numOfQwords)];
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            _sub(n2, n1, result);
            if (compareAbs(n1, n2) == 1)
            {
                result.negative = 1;
            }
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            result.negative = 1;
            _add(n1, n2, result);
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            result.negative = 0;
            _add(n1, n2, result);
        }
        else
        {
            // n1 and n2 are positive
            _sub(n1, n2, result);
            if (compareAbs(n1, n2) == -1)
            {
                result.negative = 1;
            }
        }
        break;
    case '*':
        result.negative = n1.negative & n2.negative;
        _mul(n1, n2, result);
        break;
    case '/':
        result.negative = n1.negative & n2.negative;
        _div(n1, n2, result);
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