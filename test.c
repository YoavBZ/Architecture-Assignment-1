#include <stdio.h>

extern int _add(int a, int b);
extern int _sub(int a, int b);
extern int _mul(int a, int b);
extern int _div(int a, int b);

int main(int argc, char **argv)
{
	int stack[1024];
	int d = 0, num = 0, index = 0, c = 0, positive = 1;
	while ((c = fgetc(stdin)) != 'q' && c != EOF)
	{
		switch (c)
		{
		case '0' ... '9':
			if (positive)
				num = num * 10 + (c - '0');
			else
				num = num * 10 - (c - '0');
			break;

		case '_':
			positive = 0;
			break;

		case '\0' ... ' ':
			stack[index++] = num;
			num = 0;
			break;

		case '+':
			stack[index - 2] = stack[index - 2] + stack[index - 1];
			stack[index - 1] = 0;
			index--;
			break;

		case '-':
			stack[index - 2] = stack[index - 2] - stack[index - 1];
			stack[--index] = 0;
			break;

		case '*':
			stack[index - 2] = stack[index - 2] * stack[index - 1];
			stack[--index] = 0;
			break;

		case '/':
			stack[index - 2] = stack[index - 2] / stack[index - 1];
			stack[--index] = 0;
			break;

		case 'c':
			while (index > 0)
				stack[--index] = 0;
			break;

		case 'p':
			printf("%d", stack[index - 1]);
			break;
		}
	}

	return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// extern long addDigit(long cell, char c, long *a);

// void convertBignum(char *n, long *longs)
// {
//   int len = strlen(n);
//   for (int i = len; i >= 0; i -= 9)
//   {
//     char num[9];
//     for (int j = 0; j < 9; j++)
//     {
//       num[9 - j - 1] = n[i - j - 1] != 0 ? n[i - j - 1] : '0';
//     }
//     longs[(len - i) / 9] = atol(num);
//   }
// }

// int main()
// {
//   char *str = "23076874923988036237039187093385";
//   int len = strlen(str), index = 0;
//   long longs[10];
//   // convertBignum(str, longs);
//   long cell = 0;
//   for (char i = 0; i < len; i++)
//   {
//     long *result = malloc(8);
//     long carry = addDigit(cell, str[i], result);
//     longs[index] = *result;
//     if (carry)
//       index++;
//   }
//   return 0;
// }