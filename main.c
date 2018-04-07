#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Bignum
{
    long numOfBytes;
    unsigned char *value;
    char negative;
} Bignum;

typedef struct Stack
{
    int size;
    Bignum stack[1024];
} Stack;

Stack s;

extern unsigned char mulByTen(unsigned char *value);
extern unsigned char addRecursively(unsigned char *value, short toAdd, long loopCounter);
extern void _add(unsigned char *n1, unsigned char *n2, unsigned char *result, long min, long max);
extern void _sub(unsigned char *n1, unsigned char *n2, unsigned char *result, long min, long max);
extern void _mul(unsigned char *n1, unsigned char *n2, unsigned char *result, long num1, long num2);
extern void _div(Bignum *n1, Bignum *n2, Bignum *result, Bignum *factor);

void trimBignum(Bignum *n)
{
    long originalNum = n->numOfBytes;
    for (int i = n->numOfBytes - 1; i > 0; i--)
    {
        if (n->value[i] == 0)
        {
            n->numOfBytes--;
        }
        else
        {
            break;
        }
    }
    if (originalNum != n->numOfBytes)
    {
        n->value = realloc(n->value, n->numOfBytes);
    }
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
    return s.stack[s.size - 1];
}

void freeStack()
{
    for (int i = 0; i < s.size; i++)
    {
        free(s.stack[i].value);
    }
    s.size = 0;
}

void addByteIfNeeded(Bignum *n, unsigned char next)
{
    if (next > 0)
    {
        n->numOfBytes++;
        n->value = realloc(n->value, n->numOfBytes);
        n->value[n->numOfBytes - 1] = next;
    }
}

long max(long x, long y)
{
    return x > y ? x : y;
}

// Return 1 if |n1|>|n2|, -1 if |n1|<|n2|, and 0 if there're equal
char compareAbs(Bignum *n1, Bignum *n2)
{
    if (n1->numOfBytes > n2->numOfBytes)
    {
        return 1;
    }
    else if (n1->numOfBytes < n2->numOfBytes)
    {
        return -1;
    }
    for (int i = n1->numOfBytes - 1; i >= 0; i--)
    {
        if (n1->value[i] != n2->value[i])
            return n1->value[i] > n2->value[i] ? 1 : -1;
    }
    return 0;
}

void mulByTenRecursively(Bignum *n)
{
    unsigned char next = 0;
    next = mulByTen(&(n->value[n->numOfBytes - 1]));
    long i = n->numOfBytes - 2;
    addByteIfNeeded(n, next);
    for (; i >= 0; i--)
    {
        // Adding ah to the next byte, won't overflow
        // n->value[i + 1] += mulByTen(&(n->value[i]));
        next = addRecursively(n->value + i + 1, mulByTen(&(n->value[i])), n->numOfBytes - i - 1);
        addByteIfNeeded(n, next);
    }
}

long min(long x, long y)
{
    return x < y ? x : y;
}

Bignum operate(Bignum *n1, Bignum *n2, char op)
{
    Bignum result;
    result.numOfBytes = n1->numOfBytes + n2->numOfBytes;
    result.negative = 0;
    result.value = calloc(result.numOfBytes, 1);
    long minNum = min(n1->numOfBytes, n2->numOfBytes);
    long maxNum = max(n1->numOfBytes, n2->numOfBytes);
    switch (op)
    {
    case '+':
        if (n1->negative && n2->negative)
        {
            // n1 and n2 are negative
            result.negative = 1;
            Bignum *bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum *smaller = (bigger->value == n1->value) ? n2 : n1;
            _add(bigger->value, smaller->value, result.value, minNum, maxNum);
        }
        else if (n1->negative)
        {
            // n1 is negative, n2 is positive
            if (compareAbs(n1, n2) == 1)
            {
                _sub(n1->value, n2->value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n2->value, n1->value, result.value, minNum, maxNum);
            }
        }
        else if (n2->negative)
        {
            // n1 is positive, n2 is negative
            if (compareAbs(n2, n1) == 1)
            {
                _sub(n2->value, n1->value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n1->value, n2->value, result.value, minNum, maxNum);
            }
        }
        else
        {
            // n1 and n2 are positive
            result.negative = 0;
            Bignum *bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum *smaller = (bigger->value == n1->value) ? n2 : n1;
            _add(bigger->value, smaller->value, result.value, minNum, maxNum);
        }
        break;
    case '-':
        if (n1->negative && n2->negative)
        {
            // n1 and n2 are negative
            if (compareAbs(n1, n2) == 1)
            {
                _sub(n1->value, n2->value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n2->value, n1->value, result.value, minNum, maxNum);
            }
        }
        else if (n1->negative)
        {
            // n1 is negative, n2 is positive
            result.negative = 1;
            Bignum *bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum *smaller = (bigger->value == n1->value) ? n2 : n1;
            _add(bigger->value, smaller->value, result.value, minNum, maxNum);
        }
        else if (n2->negative)
        {
            // n1 is positive, n2 is negative
            result.negative = 0;
            Bignum *bigger = (compareAbs(n1, n2) == 1) ? n1 : n2;
            Bignum *smaller = (bigger->value == n1->value) ? n2 : n1;
            _add(bigger->value, smaller->value, result.value, minNum, maxNum);
        }
        else
        {
            // n1 and n2 are positive
            if (compareAbs(n2, n1) == 1)
            {
                _sub(n2->value, n1->value, result.value, minNum, maxNum);
                result.negative = 1;
            }
            else
            {
                _sub(n1->value, n2->value, result.value, minNum, maxNum);
            }
        }
        break;
    case '*':
        result.negative = n1->negative != n2->negative;
        _mul(n1->value, n2->value, result.value, n1->numOfBytes, n2->numOfBytes);
        break;
    case '/':
        result.negative = n1->negative != n2->negative;
        Bignum factor;
        factor.negative = 0;
        factor.numOfBytes = 1;
        factor.value = calloc(1, 1);
        _div(n1, n2, &result, &factor);
        free(factor.value);
        break;
    }
    trimBignum(&result);
    return result;
}

void printReverse(Bignum *n, Bignum *divisor)
{
    if (n->numOfBytes == 1 && n->value[0] == 0)
    {
        return;
    }

    // Calculating n % 10 = n - (n/divisor)*divisor
    Bignum nCopy;
    nCopy.numOfBytes = n->numOfBytes;
    nCopy.negative = 0;
    nCopy.value = malloc(nCopy.numOfBytes);
    memcpy(nCopy.value, n->value, n->numOfBytes);

    Bignum temp = operate(n, divisor, '/');
    free(n->value);
    *n = temp;
    divisor->value[0] = 10; // Resetting divisor to 10
    temp = operate(&temp, divisor, '*');
    Bignum toPrint = operate(&nCopy, &temp, '-');

    free(temp.value);
    free(nCopy.value);

    printReverse(n, divisor);
    printf("%d", toPrint.value[0]);

    free(toPrint.value);
}

void printBignum(Bignum n)
{
    if (n.numOfBytes == 1 && n.value[0] == 0)
    {
        printf("0\n");
        return;
    }
    Bignum divisor;

    divisor.numOfBytes = 1;
    divisor.negative = 0;
    divisor.value = calloc(1, 1);
    divisor.value[0] = 10;

    // Copying this stack frame's n.value so it won't be distroyed
    unsigned char *copiedValue = malloc(n.numOfBytes);
    memcpy(copiedValue, n.value, n.numOfBytes);
    n.value = copiedValue;

    if (n.negative)
    {
        putchar('-');
        n.negative = 0;
    }

    printReverse(&n, &divisor);
    putchar('\n');

    free(divisor.value);
    free(n.value);
}

int main(int argc, char **argv)
{
    // Initiate stack with a single Bignum
    s.size = 0;
    Bignum n;
    n.numOfBytes = 0;
    n.negative = 0;
    unsigned char next = 0;
    int c = 0;
    while ((c = fgetc(stdin)) != 'q')
    {
        switch (c)
        {
        case '0' ... '9':
            if (n.numOfBytes == 0)
            {
                n.numOfBytes = 1;
                n.value = calloc(1, 1);
            }
            mulByTenRecursively(&n);
            next = addRecursively(n.value, c - '0', n.numOfBytes - 1);
            addByteIfNeeded(&n, next);
            break;

        case '_':
            n.negative = 1;
            break;

        case '\0' ... ' ':
            if (n.numOfBytes != 0)
            {
                push(n);
                n.numOfBytes = 0;
                n.negative = 0;
            }
            break;

        case '+':
        case '-':
        case '*':
        case '/':
            if (n.numOfBytes != 0)
            {
                push(n);
                n.numOfBytes = 0;
                n.negative = 0;
            }
            Bignum n2 = pop();
            Bignum n1 = pop();
            push(operate(&n1, &n2, c));
            free(n1.value);
            free(n2.value);
            break;

        case 'c':
            freeStack();
            break;

        case 'p':
            if (n.numOfBytes != 0)
            {
                push(n);
                n.numOfBytes = 0;
                n.negative = 0;
            }
            if (s.size > 0)
                printBignum(peak());
            else
                printf("0\n");
            break;
        }
    }
    freeStack();
    return 0;
}