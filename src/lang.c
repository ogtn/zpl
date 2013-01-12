//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static void checkKeyword(ztoken *token)
{
	const ztoken *kw = keywords;

	while(kw->id[0])
	{
		if(strcmp(token->id, kw->id) == 0)
		{
			token->type = kw->type;
			return;
		}

		kw++;
	}

	token->type = TOKEN_IDENTIFIER;
}

// sed "s|\(.\) \([_[:alpha:]]*\)|int tokenize_\2(const char *first, ztoken *token){return 1;}|" include/special_chars
int tokenize_eq(const char *first, ztoken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_COMPARE;
		res++; 
	}
	else
		token->type = TOKEN_ASSIGN;

	token->id[res] = '\0';

	return 1;
}


int tokenize_plus(const char *first, ztoken *token)
{
	token->type = TOKEN_ADD;

	return 1;
}

int tokenize_minus(const char *first, ztoken *token){return 1;}

int tokenize_div(const char *first, ztoken *token)
{
	if(first[1] == '/')
		return -1;
	else
		token->type = TOKEN_DIV;
		token->id[0] = '\0';
	return 1;
}

int tokenize_mult(const char *first, ztoken *token){return 1;}
int tokenize_and(const char *first, ztoken *token){return 1;}
int tokenize_or(const char *first, ztoken *token){return 1;}
int tokenize_xor(const char *first, ztoken *token){return 1;}
int tokenize_inf(const char *first, ztoken *token){return 1;}
int tokenize_sup(const char *first, ztoken *token){return 1;}
int tokenize_mod(const char *first, ztoken *token){return 1;}
int tokenize_dot(const char *first, ztoken *token){return 1;}
int tokenize_comma(const char *first, ztoken *token){return 1;}

int tokenize_semicolon(const char *first, ztoken *token)
{
	token->type = TOKEN_SEMICOLON;
	token->id[0] = '\0';

	return 1;
}

int tokenize_colon(const char *first, ztoken *token){return 1;}
int tokenize_tilde(const char *first, ztoken *token){return 1;}
int tokenize_not(const char *first, ztoken *token){return 1;}
int tokenize_paren_open(const char *first, ztoken *token){return 1;}
int tokenize_paren_close(const char *first, ztoken *token){return 1;}
int tokenize_curl_open(const char *first, ztoken *token){return 1;}
int tokenize_curl_close(const char *first, ztoken *token){return 1;}
int tokenize_bracket_open(const char *first, ztoken *token){return 1;}
int tokenize_bracket_close(const char *first, ztoken *token){return 1;}
int tokenize_at(const char *first, ztoken *token){return 1;}
int tokenize_dollar(const char *first, ztoken *token){return 1;}

int tokenize_dbl_quote(const char *first, ztoken *token)
{
	size_t length;
	const char *last = ++first;

	while(*last != '"' && *last)
		last++;

	if(!*last)
	{
		fprintf(stderr, "Ill formed string: encountered new line before matching \"\n");
		
		return -1;
	}

	length = last - first + 1;

	if(length >= IDENTIFIER_MAX)
	{
		fprintf(stderr, "Ill formed string: longer than %d characters\n", IDENTIFIER_MAX);
		
		return length;
	}

	snprintf(token->id, length, "%s", first);
	token->type = TOKEN_STRING;

	return length + 1;
}

int tokenize_quote(const char *first, ztoken *token)
{
	if(!isalnum(first[1]))
	{
		fprintf(stderr, "Illegal character after ', only alnums are accepted: '%c'\n", first[1]);

		return 2;
	}

	if(first[2] != '\'')
	{
		fprintf(stderr, "Missing matching '\n");
		
		return 2;
	}

	// OK: single valid character betwin '
	token->type = TOKEN_CONSTANT;
	token->id[0] = first[0];
	token->id[1] = first[1];
	token->id[2] = first[2];
	token->id[3] = '\0';

	return 3;
}

int tokenize_sharp(const char *first, ztoken *token){return 1;}
int tokenize_escape(const char *first, ztoken *token){return 1;}
int tokenize_back_quote(const char *first, ztoken *token){return 1;}

int tokenize_literal(const char *first, ztoken *token)
{
	size_t length;
	const char *last = first;

	while(isdigit(*last))
		last++;

	length = last - first;
	snprintf(token->id, length + 1, "%s", first);
	token->type = TOKEN_CONSTANT;

	return length;
}


static int getNextToken(const char **str, ztoken *token)
{
	const char *first;
	const char *last;
	int res = 0;

	// skip blank characters
	while(isblank(*(*str)))
		(*str)++;

	first = last = *str;

	// empty line
	if(!*first)
		return 0;

	// weird character
	if(!isgraph(*first))
	{
		fprintf(stderr, "Error: non printable or non ascii character outside string: '%c' (0x%x\n", *first, *first);
		(*str)++;

		return 1;
	}

	// default value
	// TODO: remove this when all possible characters will be taken care of in the switch...
	token->type = TOKEN_UNKNOWN;
	token->id[0] = *first;
	token->id[1] = '\0';
	
	switch(*first)
	{
		// sed "s|\(.\) \([_[:alpha:]]*\)|case '\1': out = tokenize_\2(first, token); break;|" include/special_chars
		case '=': res = tokenize_eq(first, token); break;
		case '+': res = tokenize_plus(first, token); break;
		case '-': res = tokenize_minus(first, token); break;
		case '/': res = tokenize_div(first, token); break;
		case '*': res = tokenize_mult(first, token); break;
		case '&': res = tokenize_and(first, token); break;
		case '|': res = tokenize_or(first, token); break;
		case '^': res = tokenize_xor(first, token); break;
		case '<': res = tokenize_inf(first, token); break;
		case '>': res = tokenize_sup(first, token); break;
		case '%': res = tokenize_mod(first, token); break;
		case '.': res = tokenize_dot(first, token); break;
		case ',': res = tokenize_comma(first, token); break;
		case ';': res = tokenize_semicolon(first, token); break;
		case ':': res = tokenize_colon(first, token); break;
		case '~': res = tokenize_tilde(first, token); break;
		case '!': res = tokenize_not(first, token); break;
		case '(': res = tokenize_paren_open(first, token); break;
		case ')': res = tokenize_paren_close(first, token); break;
		case '{': res = tokenize_curl_open(first, token); break;
		case '}': res = tokenize_curl_close(first, token); break;
		case '[': res = tokenize_bracket_open(first, token); break;
		case ']': res = tokenize_bracket_close(first, token); break;
		case '@': res = tokenize_at(first, token); break;
		case '$': res = tokenize_dollar(first, token); break;
		case '"': res = tokenize_dbl_quote(first, token); break;
		case '\'': res = tokenize_quote(first, token); break;
		case '#': res = tokenize_sharp(first, token); break;
		case '\\': res = tokenize_escape(first, token); break;
		case '`': res = tokenize_back_quote(first, token); break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			res = tokenize_literal(first, token); break;
		default:
			if(*first == '_' || isalpha(*first))
			{
				while(*last == '_' || isalnum(*last))
					last++;

				snprintf(token->id, last - first + 1, "%s", first);
				checkKeyword(token);
				res = last - first;
			}
		break;
	}
	
	// skip the rest of the line
	if(res < 0)
		return 0;
	// see next character...
	else if(res == 0)
		(*str++);
	// see next token
	else
		(*str) += res;

	return 1;
}


static void checkLine(const char *line, int lineNumber)
{
	if(strlen(line) == LINE_MAX)
	{
		fprintf(stderr, "Line %d exceed %d characters\n", lineNumber, LINE_MAX);
		exit(EXIT_FAILURE);
	}
}


static void cleanLine(const char *line)
{
	char *endl = strchr(line, '\n');

	if(endl != NULL)
		*endl = '\0';
}


static int parseLine(const char *line, int lineNumber)
{
	ztoken token = {"", TOKEN_UNKNOWN};

	printf("l%3d: %s\n", lineNumber, line);

	while(getNextToken(&line, &token))
	{
		switch(token.type)
		{
			// cat include/keywords | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_ALIAS:
			case TOKEN_AUTO:
			case TOKEN_BREAK:
			case TOKEN_CASE:
			case TOKEN_CLASS:
			case TOKEN_CONST:
			case TOKEN_CONTINUE:
			case TOKEN_CTOR:
			case TOKEN_DEFAULT:
			case TOKEN_DELETE:
			case TOKEN_DO:
			case TOKEN_DTOR:
			case TOKEN_ELSE:
			case TOKEN_ENUM:
			case TOKEN_FOR:
			case TOKEN_IF:
			case TOKEN_IMPORT:
			case TOKEN_IN:
			case TOKEN_INLINE:
			case TOKEN_INOUT:
			case TOKEN_MATCH:
			case TOKEN_MODULE:
			case TOKEN_NEW:
			case TOKEN_NODEFAULT:
			case TOKEN_NULL:
			case TOKEN_OUT:
			case TOKEN_PARTIAL:
			case TOKEN_PRIV:
			case TOKEN_PRIVATE:
			case TOKEN_PUB:
			case TOKEN_PUBLIC:
			case TOKEN_RETURN:
			case TOKEN_SIZEOF:
			case TOKEN_STATIC:
			case TOKEN_SWITCH:
			case TOKEN_TEMPLATE:
			case TOKEN_THIS:
			case TOKEN_TYPEDEF:
			case TOKEN_TYPEOF:
			case TOKEN_UNION:
			case TOKEN_WHILE:
				printf("keyword %s\n", token.id); break;
			// cat include/types | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_BOOL:
			case TOKEN_BYTE:
			case TOKEN_CHAR:
			case TOKEN_CHAR_16:
			case TOKEN_CHAR_32:
			case TOKEN_CHAR_8:
			case TOKEN_DOUBLE:
			case TOKEN_FLOAT:
			case TOKEN_FLOAT_16:
			case TOKEN_FLOAT_32:
			case TOKEN_FLOAT_64:
			case TOKEN_HALF:
			case TOKEN_INT:
			case TOKEN_INT_16:
			case TOKEN_INT_32:
			case TOKEN_INT_64:
			case TOKEN_INT_8:
			case TOKEN_LONG:
			case TOKEN_SHORT:
			case TOKEN_UBYTE:
			case TOKEN_UCHAR:
			case TOKEN_UCHAR_16:
			case TOKEN_UCHAR_32:
			case TOKEN_UCHAR_8:
			case TOKEN_UINT:
			case TOKEN_UINT_16:
			case TOKEN_UINT_32:
			case TOKEN_UINT_64:
			case TOKEN_UINT_8:
			case TOKEN_ULONG:
			case TOKEN_USHORT:
			case TOKEN_VOID:
				printf("native type %s\n", token.id); break;
			case TOKEN_COMPARE:
			case TOKEN_DIV:
			case TOKEN_ADD:
			case TOKEN_ASSIGN:
				printf("operator %s\n", token.id); break;
			case TOKEN_IDENTIFIER:
				printf("identifier '%s'\n", token.id); break;
			case TOKEN_SEMICOLON:
				printf("semicolon\n"); break;
			case TOKEN_STRING:
				printf("string '%s'\n", token.id); break;
			case TOKEN_CONSTANT:
				printf("constant '%s'\n", token.id); break;
			case TOKEN_UNKNOWN: default:
				printf("Found unknown token(%d) id: '%s'\n", token.type, token.id);
				return -1;
		}
	}

	return 0;
}


int main(int argc, char **argv)
{
	char line[LINE_MAX + 1];
	int status = EXIT_SUCCESS;
	int i;

	// check args
	if(argc <= 1)
	{
		fprintf(stderr, "No source file specified\nlang files\n");
		exit(EXIT_FAILURE);
	}

	// foreach file
	for(i = 1; i < argc; i++)
	{
		char *fileName = argv[i];
		int lineNumber = 1;
		FILE *f;

		// opening source file
		f = fopen(fileName, "r");

		if(f == NULL)
		{
			fprintf(stderr, "Unable to load file '%s'\n", fileName);
			continue;
		}

		printf("##### Parsing file '%s' #####\n", fileName);

		while(fgets(line, LINE_MAX + 1, f))
		{
			checkLine(line, lineNumber);
			cleanLine(line);
			
			if(parseLine(line, lineNumber) != 0)
			{
				status = EXIT_FAILURE;
				break;
			}

			lineNumber++;
		}

		if(status == EXIT_SUCCESS)
			printf("##### Parsing of file '%s' completed #####\n\n", fileName);
		else
			printf("##### Parsing of file '%s' Failed #####\n\n", fileName);

		fclose(f);
	}

	return status;
}
