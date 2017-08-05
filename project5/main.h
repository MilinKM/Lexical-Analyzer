#ifndef MAIN_H
#define MAIN_H

/* Header include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* user defined macros */
#define FAILURE -1
#define SUCCESS 0
#define SET 1
#define RESET 0

/* global Declarations */
char line_no, headerfile_flag;
char string_buff [100], temp_buff[100];
char buffer [2048];
char start_string;

/* user defined enums for States */
typedef enum States
{
	s0_idle,
	s1_comment,
	s2_preprocessor,
	s3_headerfile,
	s4_res_keyword_data,
	s5_res_keyword_non_data,
	s6_string,
	s7_numeric_constant,
	s8_ASCII_char
}State_e;

/* user defined enums for Events */
typedef enum Events
{
	PEVENT_NULL,
	PEVENT_SINGLE_LINE_COMMENT,
	PEVENT_MULTI_LINE_COMMENT,
	PEVENT_PREPROCESSOR_DIRECTIVE,
	PEVENT_HEADERFILE,
	PEVENT_RESERVED_KEYWORD,
	PEVENT_STRING,
	PEVENT_NUMERIC_CONSTANT,
	PEVENT_ASCII_CHAR,
	PEVENT_EOF
}Event_e;


/* user defined function prototype */
int lex_analyzer(FILE *src_fd);
int error_check(FILE *);

#endif
