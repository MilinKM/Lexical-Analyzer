#include <stdio.h>
#include "main.h"
int main(int argc, char *argv[])
{
	// Error checking
	if (argc != 2)
	{
		printf("./<exectable_file> <source_file>\n");
		return FAILURE;
	}
	if (argv[1] == NULL)
	{
		printf("ERROR: fail to open the file\n");
		return FAILURE;
	}
	FILE *ptr;
	ptr = fopen(argv[1], "r");
	// calling Error checking function for generating tokens
	int result = error_check(ptr);
	if (result == FAILURE)
	{
		printf("ERROR: closing paranthesis is missing\n");
		return FAILURE;
	}
	fseek(ptr,0l,SEEK_SET);
	// calling lex_analyzer function for generating tokens
	lex_analyzer(ptr);

}
