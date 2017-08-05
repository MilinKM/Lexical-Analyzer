#include "main.h"
int lex_analyzer(FILE *src_fp)
{ /* definations */
	char *l = "-n";
	char *reserved_key1[] = {"const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned", "short", "long", "double", "char", "int", "float", "struct", "union", "enum", "void", "typedef", "FILE"};
	char *reserved_key2[] = {"goto", "return", "continue", "break", "if", "else", "for", "while", "do", "switch", "case", "default", "sizeof", "EOF", "stderr", "stdout", "stdin"};

	int previous_state, present_state, event = 0;
	previous_state = s0_idle;
	present_state = s0_idle;
	int temp = 0, kdx = 0;
	char str[20];
	int pdx = 0;
	char test1 = 0, test2 = 0, test6 = 0, test7 = 0;
	char *ptr = (char *)&temp;
	int idx = 0, jdx = 1;
	int test3 = 0, test4 = 0, test5 = 0;
	while ((temp = fgetc(src_fp)) != EOF)
	{
		if ( present_state == s0_idle && *ptr == '/')
		{
			test1 = *ptr;
			present_state = s1_comment;
			continue;
		}
		if ((event == PEVENT_MULTI_LINE_COMMENT) && (*ptr == '*') )
		{
			test2 = *ptr;
			continue;
		}
		// checking for preprocessor	
		if (present_state == s0_idle && *ptr == '#')
		{
			present_state = s2_preprocessor;
		}
		// checking for header_file
		if ( previous_state == s2_preprocessor && (*ptr == '<' || *ptr == '"'))
		{
			present_state = s3_headerfile;
			if (*ptr == '"')
			{
				previous_state = present_state;
				buffer[idx] = *ptr;
				idx++;
				continue;
			}
		}
		// checking for comments
		if (present_state == s1_comment && (((test1 == '/') && (*ptr == '*')) || ((test1 == '/') && (*ptr == '/'))))
		{
			buffer[idx] = test1;
			idx++;
			if (test1 == '/' && *ptr == '*')
				event = PEVENT_MULTI_LINE_COMMENT;
			else if (test1 == '/' && *ptr == '/')
				event = PEVENT_SINGLE_LINE_COMMENT;
			test1 = 0;
		}
		// for reserved data    &
		// numeric constant
		if (present_state == s0_idle)
		{
			switch (*ptr)
			{
				case 'a' ... 'z' :
				case 'A' ... 'Z':
					present_state = s4_res_keyword_data;
					break;
				case '0' ... '9' :
					present_state = s7_numeric_constant;
					break;
			}
		}

		// for strings
		if (present_state == s0_idle && *ptr == '"')
		{
			present_state = s6_string;
			printf("%-30c : Double_quotation\n", *ptr);
			continue;
		} 

		// for character
		if (present_state == s0_idle && *ptr == 39)
		{
			present_state = s8_ASCII_char;
			test7 = *ptr;
			printf("%-30c : Single_quotation\n", test7);
			continue;
		}
		// Generating token for preprocessor
		if (present_state == s2_preprocessor)
		{
			if (*ptr == 32 || *ptr == '<' || *ptr == '"')
			{
				buffer[idx] = '\0';
				idx = 0;
				fseek(src_fp,-1l,SEEK_CUR);
				printf("%-30s : preprocessor\n", buffer);
				previous_state = present_state;
				if (*ptr == 32)
					present_state = s0_idle;
				continue;
			}
			else
			{
				buffer[idx] = *ptr;
				idx++;
			}

		}
		// Generating token for Header file

		if (present_state == s3_headerfile)
		{
			if (*ptr == '>' || *ptr == '"')
			{
				buffer[idx] = *ptr;
				buffer[idx + 1] = '\0';
				idx = 0;
				printf("%-30s : Header_file\n", buffer);
				previous_state = present_state;
				present_state = s0_idle;
				continue;
			}
			else 
			{
				buffer[idx] = *ptr;
				idx++;
			}
		}
		// Generating token for Comments

		if (present_state == s1_comment)
		{
			if (event == PEVENT_MULTI_LINE_COMMENT)
			{
				if (test2 == '*' && *ptr == '/')
				{
					buffer[idx] = test2;
					test2 = 0;
					buffer[idx + 1] = *ptr;
					buffer[idx + 2] = '\0';
					idx = 0;
					printf("%-30s : Multi_line_comment\n", buffer);
					previous_state = present_state;
					present_state = s0_idle;
					continue;
				}
				else
				{
					buffer[idx] = *ptr;
					idx++;
				}
			}
			else if (event == PEVENT_SINGLE_LINE_COMMENT)
			{
				if (*ptr == '\n')
				{
					buffer[idx] = '\0';
					printf("%-30s : single_line_comment\n", buffer);
					previous_state = present_state;
					present_state = s0_idle;
					idx = 0;
				}
				else
				{
					buffer[idx] = *ptr;
					idx++;
				}
			}
		}
		// Generating token for rserved key words

		if (present_state == s4_res_keyword_data)
		{
			if (*ptr == 32 || *ptr == '[' || *ptr == ']' || *ptr == '(' || *ptr == ')' || *ptr == ';' || *ptr == '\n')
			{
				buffer[idx] = '\0';
				for (kdx = 0; kdx < 20; kdx++)
				{
					if (!strcmp(buffer,reserved_key1[kdx]))
					{
						test4 = 1;
						break;
					}
				}
				if (test4 != 1)
				{
					for (kdx = 0; kdx < 17; kdx++)
					{
						if (!strcmp(buffer,reserved_key2[kdx]))
						{
							test5 = 1;
							break;
						}
					}
				}
				if (test4 == 1)
				{
					printf("%-30s : Data_type\n", buffer);
					test4 = 0;
				}
				else if (test5 == 1)
				{
					printf("%-30s : Reserved_key_word\n", buffer);
					test5 = 0;
				}
				else
					printf("%-30s : Identifier\n", buffer);
				previous_state = present_state;
				present_state = s0_idle;
				idx = 0;
				fseek(src_fp,-1l,SEEK_CUR);
				continue;
			}
			else
			{
				buffer[idx] = *ptr;
				idx++;
			}
		}
		if (present_state == s6_string && (*ptr == '%' || *ptr == '\n'))
		{
			if (*ptr == '%')
			{
				test6 = *ptr;
				continue;
			}
			else if (*ptr == '\n')
				test6 = *ptr;
		}

		if (present_state == s6_string)
		{
			if (*ptr == '"')
			{
				buffer[idx] = '\0';
				printf("%-30s : string\n", buffer);
				printf("%-30c : Double_quotation\n", *ptr);
				previous_state = present_state;
				present_state = s0_idle;
				idx = 0;
				continue;
			}
			else
			{
				if (test6 == '%' || test6 == '\n')
				{
					if (test6 == '%')
					{
						str[pdx] = test6;
						str[pdx + 1] = *ptr;
						str[pdx + 2] = '\0';
					}
					else if (test6 == '\n')
					{
						str[pdx] = test6;
						str[pdx + 1] = '\0';
					}
					test6 = 0;
					pdx = 0;
					buffer[idx] = '\0';
					idx = 0;
					printf("%-30s : string\n", buffer);
					printf("%-30s : format_strings\n", str);
				}
				else
				{
					buffer[idx] = *ptr;
					idx++;
				}
			}

		}

		if (present_state == s7_numeric_constant)
		{
			if (*ptr == 32 || *ptr == ','|| *ptr == ')' || *ptr == ']' || *ptr == '}' || *ptr == ';')
			{
				buffer[idx] = '\0';
				printf("%-30s : digit\n", buffer);
				fseek(src_fp,-1l,SEEK_CUR);
				previous_state = present_state;
				present_state = s0_idle;
				idx = 0;
				continue;
			}
			else
			{
				buffer[idx] = *ptr;
				idx++;
			}
		}
		if (present_state == s8_ASCII_char)
		{
			if (*ptr == 39)
			{
				buffer[idx] = '\0';
				printf("%-30s : Ascii_char\n", buffer);
				printf("%-30c : Single_quotation\n", *ptr);
				previous_state = present_state;
				present_state = s0_idle;
				idx = 0;
				test7 = 0;
				continue;
			}
			else
			{
				buffer[idx] = *ptr;
				idx++;
			}
		}
		// for idle case
		if (present_state == s0_idle)
		{
			switch (*ptr)
			{
				case '(' : 
				case '{':
				case '[':
					printf("%-30c : Open_braces\n", *ptr);
					break;
				case ')':
				case '}':
				case ']':
					printf("%-30c : Close_braces\n", *ptr);
					break;
				case ';':
					printf("%-30c : End of statement\n", *ptr);
					break;
				case ',':
					printf("%-30c : Comma operator\n", *ptr);
					break;
				case '=':
					printf("%-30c : Assignment Operator\n", *ptr);
					break;
			}
		} 

	}
}
