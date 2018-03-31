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
    Bignum stack[1024];
} Stack;

Stack s;

extern char mulByTen(char *value);
extern char addRecursively(char *value, short toAdd, long loopCounter);
extern void _add(char *n1, char *n2, char *result, long min, long max);
extern void _sub(char *n1, char *n2, char *result, long min, long max);
extern void _mul(char *n1, char *n2, char *result, long num1, long num2);
extern void _div(char *n1, char *n2, char *result, long min);

void printBignum(Bignum *n)
{
    // for (int i = 0; i < n->number_of_digits; i++)
    // putchar(n->digits[i]);
    putchar('\n');
}

void push(Bignum n)
{
    s.stack[s.size++] = n;
}

Bignum pop()
{
    return s.stack[--s.size];
}

Bignum peak()
{
    return s.stack[s.size];
}

void freeStack()
{
    for (int i = 0; i < s.size; i++)
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

char max(long x, long y)
{
    return x > y ? x : y;
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

Bignum trimBignum(Bignum n)
{
    long originalNum = n.numOfQwords;
    for (int i = n.numOfQwords - 1; i >= 0; i--)
    {
        if (n.value[i] == 0)
        {
            n.numOfQwords--;
        }
    }
    if (originalNum != n.numOfQwords)
    {
        n.value = realloc(n.value, n.numOfQwords);
    }
    return n;
}

long min(long x, long y)
{
    return x < y ? x : y;
}

Bignum operate(Bignum n1, Bignum n2, char op)
{
    Bignum result;
    result.numOfQwords = n1.numOfQwords + n2.numOfQwords;
    result.negative = 0;
    result.value = malloc(result.numOfQwords);
    long minNum = min(n1.numOfQwords, n2.numOfQwords);
    long maxNum = max(n1.numOfQwords, n2.numOfQwords);
    switch (op)
    {
    case '+':
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            result.negative = 1;
            Bignum bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum smaller = (bigger.value == n1.value) ? n2 : n1;
            _add(bigger.value, smaller.value, result.value, minNum, maxNum);
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            if (compareAbs(n1, n2) == 1)
            {
                _sub(n1.value, n2.value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n2.value, n1.value, result.value, minNum, maxNum);
            }
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            if (compareAbs(n2, n1) == 1)
            {
                _sub(n2.value, n1.value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n1.value, n2.value, result.value, minNum, maxNum);
            }
        }
        else
        {
            // n1 and n2 are positive
            result.negative = 0;
            Bignum bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum smaller = (bigger.value == n1.value) ? n2 : n1;
            _add(bigger.value, smaller.value, result.value, minNum, maxNum);
        }
        break;
    case '-':
        if (n1.negative && n2.negative)
        {
            // n1 and n2 are negative
            if (compareAbs(n1, n2) == 1)
            {
                _sub(n1.value, n2.value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n2.value, n1.value, result.value, minNum, maxNum);
            }
        }
        else if (n1.negative)
        {
            // n1 is negative, n2 is positive
            result.negative = 1;
            Bignum bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum smaller = (bigger.value == n1.value) ? n2 : n1;
            _add(bigger.value, smaller.value, result.value, minNum, maxNum);
        }
        else if (n2.negative)
        {
            // n1 is positive, n2 is negative
            result.negative = 0;
            Bignum bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum smaller = (bigger.value == n1.value) ? n2 : n1;
            _add(bigger.value, smaller.value, result.value, minNum, maxNum);
        }
        else
        {
            // n1 and n2 are positive
            if (compareAbs(n2, n1) == 1)
            {
                _sub(n2.value, n1.value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n1.value, n2.value, result.value, minNum, maxNum);
            }
        }
        break;
    case '*':
        result.negative = n1.negative != n2.negative;
        _mul(n1.value, n2.value, result.value, n1.numOfQwords, n2.numOfQwords);
        break;
    case '/':
        result.negative = n1.negative & n2.negative;
        _div(n1.value, n2.value, result.value, result.numOfQwords);
        break;
    }
    return trimBignum(result);
}

int main(int argc, char **argv)
{
    // Initiate stack with a single Bignum
    s.size = 0;
    Bignum n;
    n.numOfQwords = 0;
    n.value = malloc(1);
    char next = 0;
    int c = 0;
    while ((c = fgetc(stdin)) != 'q')
    {
        switch (c)
        {
        case '0' ... '9':
            if (n.numOfQwords == 0)
            {
                n.numOfQwords = 1;
                n.value = malloc(1);
            }
            mulByTenRecursively(&n);
            next = addRecursively(n.value, c - '0', n.numOfQwords - 1);
            addQwordIfNeeded(&n, next);
            break;

        case '_':
            n.negative = 1;
            break;

        case '\0' ... ' ':
            if (n.numOfQwords != 0)
            {
                push(n);
                n.numOfQwords = 0;
                n.negative = 0;
            }
            break;

        case '+':
        case '-':
        case '*':
        case '/':
            if (n.numOfQwords != 0)
            {
                push(n);
            }
            Bignum n2 = pop();
            Bignum n1 = pop();
            push(operate(n1, n2, c));
            free(n1.value);
            free(n2.value);
            break;

        case 'p':
            // printBignum(peak(s));
            break;
        }
    }
    freeStack();
    return 0;
}