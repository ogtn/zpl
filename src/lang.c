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


static void checkKeyword(ztoken *token, const char *word)
{
	int i = 0;

	// look for keywords
	while(all_keywords[i])
	{
		if(strcmp(word, all_keywords[i]) == 0)
		{
			token->type = i;
			
			return;
		}

		i++;
	}

	i = 0;

	// look for types
	while(all_types[i])
	{
		if(strcmp(word, all_types[i]) == 0)
		{
			token->type = TOKEN_TYPE;
			token->data.type = i;
			
			return;
		}

		i++;
	}

	// if no previous match, it's an identifier
	token->type = TOKEN_IDENTIFIER;
	token->data.str_val = malloc(strlen(word) + 1);
	strcpy(token->data.str_val, word);
}

// sed "s|\(.\) \([_[:alpha:]]*\)|int tokenize_\2(const char *first, ztoken *token){return 1;}|" include/special_chars
static int tokenize_eq(const char *first, ztoken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_COMPARE;
		res++; 
	}
	else
		token->type = TOKEN_ASSIGN;

	return 1;
}


static int tokenize_plus(const char *first, ztoken *token)
{
	token->type = TOKEN_ADD;

	return 1;
}


static int tokenize_minus(const char *first, ztoken *token)
{
	token->type = TOKEN_MINUS;

	return 1;
}


static int tokenize_div(const char *first, ztoken *token)
{
	if(first[1] == '/')
		return -1;
	else
		token->type = TOKEN_DIV;

	return 1;
}


static int tokenize_mult(const char *first, ztoken *token)
{
	token->type = TOKEN_MULT;

	return 1;
}


static int tokenize_and(const char *first, ztoken *token)
{
	int res = 1;

	if(first[1] == '&')
	{
		token->type = TOKEN_BIT_AND;
		res++;
	}
	else
		token->type = TOKEN_AND;

	return 1;
}


static int tokenize_or(const char *first, ztoken *token)
{
	int res = 1;

	if(first[1] == '|')
	{
		token->type = TOKEN_BIT_OR;
		res++;
	}
	else
		token->type = TOKEN_OR;

	return res;
}


static int tokenize_xor(const char *first, ztoken *token)
{
	token->type = TOKEN_XOR;

	return 1;
}


static int tokenize_inf(const char *first, ztoken *token)
{
	token->type = TOKEN_INF;

	return 1;
}

static int tokenize_sup(const char *first, ztoken *token)
{
	token->type = TOKEN_SUP;

	return 1;
}


static int tokenize_mod(const char *first, ztoken *token)
{
	token->type = TOKEN_MOD;

	return 1;
}


static int tokenize_dot(const char *first, ztoken *token)
{
	token->type = TOKEN_DOT;

	return 1;
}


static int tokenize_comma(const char *first, ztoken *token)
{
	token->type = TOKEN_COMMA;

	return 1;
}


static int tokenize_semicolon(const char *first, ztoken *token)
{
	token->type = TOKEN_SEMICOLON;

	return 1;
}


static int tokenize_colon(const char *first, ztoken *token)
{
	token->type = TOKEN_COLON;

	return 1;
}


static int tokenize_tilde(const char *first, ztoken *token)
{
	token->type = TOKEN_TILDE;

	return 1;
}


static int tokenize_not(const char *first, ztoken *token)
{
	token->type = TOKEN_NOT;

	return 1;
}


static int tokenize_paren_open(const char *first, ztoken *token)
{
	token->type = TOKEN_OPEN_PAREN;

	return 1;
}


static int tokenize_paren_close(const char *first, ztoken *token)
{
	token->type = TOKEN_CLOSE_PAREN;

	return 1;
}


static int tokenize_curl_open(const char *first, ztoken *token)
{
	token->type = TOKEN_OPEN_BLOCK;

	return 1;
}


static int tokenize_curl_close(const char *first, ztoken *token)
{
	token->type = TOKEN_OPEN_BLOCK;

	return 1;
}


static int tokenize_bracket_open(const char *first, ztoken *token)
{
	token->type = TOKEN_OPEN_BRACKET;

	return 1;
}


static int tokenize_bracket_close(const char *first, ztoken *token)
{
	token->type = TOKEN_OPEN_BRACKET;

	return 1;
}


static int tokenize_at(const char *first, ztoken *token)
{
	token->type = TOKEN_AT;

	return 1;
}


static int tokenize_dollar(const char *first, ztoken *token)
{
	token->type = TOKEN_DOLLAR;

	return 1;
}


static int tokenize_dbl_quote(const char *first, ztoken *token)
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

	token->data.str_val = malloc(length + 1);
	snprintf(token->data.str_val, length, "%s", first);
	token->type = TOKEN_STRING;

	return length + 1;
}


static int tokenize_quote(const char *first, ztoken *token)
{
	if(!isprint(first[1]))
	{
		fprintf(stderr, "Illegal character after ', only printable characters are accepted: '%c'\n", first[1]);

		return 2;
	}

	if(first[2] != '\'')
	{
		fprintf(stderr, "Missing matching '\n");
		
		return 2;
	}

	// OK: single valid character betwin '
	token->type = TOKEN_LITERAL_CHAR;
	token->data.char_val = first[1];

	return 3;
}


static int tokenize_sharp(const char *first, ztoken *token)
{
	token->type = TOKEN_SHARP;

	return 1;
}


static int tokenize_escape(const char *first, ztoken *token)
{
	token->type = TOKEN_ESCAPE;

	return 1;
}


static int tokenize_back_quote(const char *first, ztoken *token)
{
	token->type = TOKEN_BACK_QUOTE;

	return 1;
}


static int tokenize_literal(const char *first, ztoken *token)
{
	const char *last = first;

	while(isdigit(*last))
		last++;

	token->data.int_val = atoi(first);
	token->type = TOKEN_LITERAL_INT;

	return last - first;
}


static int tokenize_identifier(const char *first, ztoken *token)
{
	char word[IDENTIFIER_MAX + 1];
	const char *last = first;
	int res;

	if(isdigit(*first))
	{
		res = tokenize_literal(first, token);
	}
	else if(*first == '_' || isalpha(*first))
	{
		while(*last == '_' || isalnum(*last))
			last++;

		snprintf(word, last - first + 1, "%s", first);
		checkKeyword(token, word);
		res = last - first;
	}

	return res;
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
	token->data.char_val = *first;
	
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
		default: res = tokenize_identifier(first, token); break;
	}
	
	// skip the rest of the line
	if(res < 0)
		return 0;
	// see next character...
	else if(res == 0)
		(*str)++;
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
	int error_cpt = 0;
	ztoken token;

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
				printf("keyword %s\n", all_keywords[token.type]); break;
			// cat include/types | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_TYPE:
				printf("type %s\n", all_types[token.data.type]); break;
			case TOKEN_COMPARE:
			case TOKEN_DIV:
			case TOKEN_ADD:
			case TOKEN_ASSIGN:
			case TOKEN_MINUS:
			case TOKEN_MULT:
			case TOKEN_AND:
			case TOKEN_BIT_AND:
			case TOKEN_OR:
			case TOKEN_BIT_OR:
			case TOKEN_XOR:
			case TOKEN_DOT:
			case TOKEN_COMMA:
			case TOKEN_COLON:
			case TOKEN_INF:
			case TOKEN_SUP:
			case TOKEN_MOD:
			case TOKEN_TILDE:
			case TOKEN_NOT:
			case TOKEN_OPEN_PAREN:
			case TOKEN_CLOSE_PAREN:
			case TOKEN_OPEN_BRACKET:
			case TOKEN_CLOSE_BRACKET:
			case TOKEN_AT:
			case TOKEN_DOLLAR:
			case TOKEN_SHARP:
			case TOKEN_ESCAPE:
			case TOKEN_BACK_QUOTE:
				printf("operator %c\n", token.data.char_val); break;
			case TOKEN_OPEN_BLOCK:
				printf("Block opened\n"); break;
			case TOKEN_CLOSE_BLOCK:
				printf("Block closed\n"); break;
			case TOKEN_IDENTIFIER:
				printf("identifier '%s'\n", token.data.str_val); break;
			case TOKEN_SEMICOLON:
				printf("semicolon\n"); break;
			case TOKEN_STRING:
				printf("string '%s'\n", token.data.str_val); break;
			case TOKEN_LITERAL_CHAR:
				printf("constant %d\n", token.data.char_val); break;
			case TOKEN_LITERAL_INT:
				printf("constant %d\n", token.data.int_val); break;
			case TOKEN_UNKNOWN: default:
				printf("Found unknown token(%d): '%c'\n", token.type, token.data.char_val);
				error_cpt++;
		}
	}

	return error_cpt;
}


int main(int argc, char **argv)
{
	char line[LINE_MAX + 1];
	int error_cpt = 0;
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
			error_cpt += parseLine(line, lineNumber);
			lineNumber++;
		}

		if(error_cpt == 0)
			printf("##### Parsing of file '%s' completed #####\n\n", fileName);
		else
			printf("##### Parsing of file '%s' Failed: %d errors #####\n\n", fileName, error_cpt);

		fclose(f);
	}

	return -error_cpt;
}
