#include "main.h"
int error_check(FILE *ptr)
{
	int temp;
	int count = 0, quotation = 0;
	while ((temp = fgetc(ptr)) != EOF)
	{
		if (temp == '(' || temp == '[' || temp == '{' || temp == '"')
		{
			if (temp == '"')
				quotation++;
			else
			count++;
		}
		if (temp == ')' || temp == ']' || temp == '}')
			count--;
	}
	if (count != 0 || ((quotation % 2) != 0))
		return FAILURE;
}
