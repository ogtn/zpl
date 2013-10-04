//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "tokenizer.h"
#include "lang.h"
#include "zcompiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


extern zContext context;


static void checkKeyword(zToken *token, const char *word)
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


// sed "s|\(.\) \([_[:alpha:]]*\)|int tokenize_\2(const char *first, zToken *token){return 1;}|" include/special_chars
static int tokenize_eq(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_COMPARE;
		res++;
	}
	else
		token->type = TOKEN_ASSIGN;

	return res;
}


static int tokenize_plus(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '=':
			token->type = TOKEN_ADD_ASSIGN;
			res++;
			break;

		case '+':
			token->type = TOKEN_INCR;
			res++;
			break;

		default:
			token->type = TOKEN_ADD;
	}

	return res;
}


static int tokenize_minus(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '=':
			token->type = TOKEN_SUB_ASSIGN;
			res++;
			break;

		case '-':
			token->type = TOKEN_DECR;
			res++;
			break;

		default:
			token->type = TOKEN_SUB;
	}

	return res;
}


static int tokenize_div(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '/':
			res++;

			while(first[res] && first[res] != '\n')
				res++;

			token->type = TOKEN_COMMENT;
			token->data.str_val = malloc(res -1);
			memcpy(token->data.str_val, first + 2, res - 2);
			token->data.str_val[res - 2] = '\0';

			return res;

		case '=':
			token->type = TOKEN_DIV_ASSIGN;
			res++;
			break;

		case '*':
			while(first[res])
			{
				if(first[res + 1] == '*' && first[res + 2] == '/')
				{
					token->type = TOKEN_COMMENT;
					token->data.str_val = malloc(res);
					memcpy(token->data.str_val, first + 2, res - 1);
					token->data.str_val[res - 1] = '\0';

					return res + 3;
				}

				res++;
			}

			fprintf(stderr, "Non closed C-style comment: end of file reached before '*/'\n");
			return -1;

		default:
			token->type = TOKEN_DIV;
	}

	return res;
}


static int tokenize_mult(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_MULT_ASSIGN;
		res++;
	}
	else
		token->type = TOKEN_MULT;

	return res;
}


static int tokenize_and(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '&':
			token->type = TOKEN_AND;
			res++;
			break;

		case '=':
			token->type = TOKEN_BIT_AND_ASSIGN;
			res++;
			break;

		default:
			token->type = TOKEN_BIT_AND;
	}

	return res;
}


static int tokenize_or(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '|':
			token->type = TOKEN_OR;
			res++;
			break;

		case '=':
			token->type = TOKEN_BIT_OR_ASSIGN;
			res++;
			break;

		default:
			token->type = TOKEN_BIT_OR;
	}


	return res;
}


static int tokenize_xor(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_XOR_ASSIGN;
		res++;
	}
	else
		token->type = TOKEN_XOR;

	return res;
}


static int tokenize_inf(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '=':
			token->type = TOKEN_INF_EQ;
			res++;
			break;

		case '<':
			if(first[2] == '=')
			{
				token->type = TOKEN_LSHIFT_ASSIGN;
				res++;
			}
			else
				token->type = TOKEN_LSHIFT;

			res++;
			break;

		default:
			token->type = TOKEN_INF;
	}

	return res;
}

static int tokenize_sup(const char *first, zToken *token)
{
	int res = 1;

	switch(first[1])
	{
		case '=':
			token->type = TOKEN_SUP_EQ;
			res++;
			break;

		case '>':
			if(first[2] == '=')
			{
				token->type = TOKEN_RSHIFT_ASSIGN;
				res++;
			}
			else
				token->type = TOKEN_RSHIFT;

			res++;
			break;

		default:
			token->type = TOKEN_SUP;
	}

	return res;
}


static int tokenize_mod(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_MOD_ASSIGN;
		res++;
	}
	else
		token->type = TOKEN_MOD;

	return res;
}


static int tokenize_dot(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '.' && first[2] == '.')
	{
		token->type = TOKEN_ELLIPSE;
		res += 2;
	}
	else
		token->type = TOKEN_DOT;

	return res;
}


static int tokenize_comma(const char *first, zToken *token)
{
	token->type = TOKEN_COMMA;

	return 1;
}


static int tokenize_semicolon(const char *first, zToken *token)
{
	token->type = TOKEN_SEMICOLON;

	return 1;
}


static int tokenize_colon(const char *first, zToken *token)
{
	token->type = TOKEN_COLON;

	return 1;
}


static int tokenize_tilde(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_BIT_NOT_ASSIGN;
		res++;
	}
	else
		token->type = TOKEN_BIT_NOT;

	return res;
}


static int tokenize_not(const char *first, zToken *token)
{
	int res = 1;

	if(first[1] == '=')
	{
		token->type = TOKEN_NOT_ASSIGN;
		res++;
	}
	else
		token->type = TOKEN_NOT;

	return res;
}


static int tokenize_paren_open(const char *first, zToken *token)
{
	token->type = TOKEN_OPEN_PAREN;

	return 1;
}


static int tokenize_paren_close(const char *first, zToken *token)
{
	token->type = TOKEN_CLOSE_PAREN;

	return 1;
}


static int tokenize_curl_open(const char *first, zToken *token)
{
	token->type = TOKEN_OPEN_BLOCK;

	return 1;
}


static int tokenize_curl_close(const char *first, zToken *token)
{
	token->type = TOKEN_OPEN_BLOCK;

	return 1;
}


static int tokenize_bracket_open(const char *first, zToken *token)
{
	token->type = TOKEN_OPEN_BRACKET;

	return 1;
}


static int tokenize_bracket_close(const char *first, zToken *token)
{
	token->type = TOKEN_OPEN_BRACKET;

	return 1;
}


static int tokenize_at(const char *first, zToken *token)
{
	token->type = TOKEN_AT;

	return 1;
}


static int tokenize_dollar(const char *first, zToken *token)
{
	token->type = TOKEN_DOLLAR;

	return 1;
}


static int tokenize_dbl_quote(const char *first, zToken *token)
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


static int tokenize_quote(const char *first, zToken *token)
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


static int tokenize_sharp(const char *first, zToken *token)
{
	token->type = TOKEN_SHARP;

	return 1;
}


static int tokenize_escape(const char *first, zToken *token)
{
	token->type = TOKEN_ESCAPE;

	return 1;
}


static int tokenize_back_quote(const char *first, zToken *token)
{
	token->type = TOKEN_BACK_QUOTE;

	return 1;
}


static int tokenize_literal(const char *first, zToken *token)
{
	const char *last = first;

	while(isdigit(*last))
		last++;

	token->data.int_val = atoi(first);
	token->type = TOKEN_LITERAL_INT;

	return last - first;
}


static int tokenize_identifier(const char *first, zToken *token)
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
	else
	{
		printf("AAAAAAAAAAAAAAAAAAAAA\n");
		res = 0;
	}

	return res;
}


int tokenize(const char *code, zToken **tokens, int *nbTokens)
{
	const char *codeStart = code;
	int maxTokens = 1024;

	*nbTokens = 0;
	*tokens = malloc(sizeof(zToken) * maxTokens);

	while(*code)
	{
		zToken *token;
		int len;

		// skip blank characters and endlines
		while(*code && (isblank(*code) || *code == '\n'))
			code++;

		// end of file
		if(!*code)
			break;

		// weird character
		if(!isgraph(*code))
		{
			fprintf(stderr, "Error: non printable or non ascii character found @ %d: (0x%hhX)\n", code - codeStart, *code);
			code++;

			continue;
		}

		if(*nbTokens >= maxTokens)
		{
			fprintf(stderr, "Error: max token reached\n");
			return -1;
		}

		token = &(*tokens)[*nbTokens];
		(*nbTokens)++;

		token->type = TOKEN_UNKNOWN;
		token->data.char_val = *code;
		token->loc.pos = code - codeStart;

		switch(*code)
		{
			// sed "s|\(.\) \([_[:alpha:]]*\)|case '\1': len = tokenize_\2(code, token); break;|" include/special_chars
			case '|': len = tokenize_or(code, token); break;
			case '=': len = tokenize_eq(code, token); break;
			case '@': len = tokenize_at(code, token); break;
			case '&': len = tokenize_and(code, token); break;
			case '^': len = tokenize_xor(code, token); break;
			case '<': len = tokenize_inf(code, token); break;
			case '>': len = tokenize_sup(code, token); break;
			case '%': len = tokenize_mod(code, token); break;
			case '.': len = tokenize_dot(code, token); break;
			case '/': len = tokenize_div(code, token); break;
			case '!': len = tokenize_not(code, token); break;
			case '*': len = tokenize_mult(code, token); break;
			case '+': len = tokenize_plus(code, token); break;
			case '#': len = tokenize_sharp(code, token); break;
			case '-': len = tokenize_minus(code, token); break;
			case ',': len = tokenize_comma(code, token); break;
			case ':': len = tokenize_colon(code, token); break;
			case '~': len = tokenize_tilde(code, token); break;
			case '$': len = tokenize_dollar(code, token); break;
			case '"': len = tokenize_dbl_quote(code, token); break;
			case '{': len = tokenize_curl_open(code, token); break;
			case ';': len = tokenize_semicolon(code, token); break;
			case '`': len = tokenize_back_quote(code, token); break;
			case '}': len = tokenize_curl_close(code, token); break;
			case '(': len = tokenize_paren_open(code, token); break;
			case ')': len = tokenize_paren_close(code, token); break;
			case '[': len = tokenize_bracket_open(code, token); break;
			case ']': len = tokenize_bracket_close(code, token); break;
			case '\'': len = tokenize_quote(code, token); break;
			case '\\': len = tokenize_escape(code, token); break;
			default: len = tokenize_identifier(code, token); break;
		}

		if(len > 0)
		{
			code += len;
			token->loc.len = len;
		}
		else
		{
			fprintf(stderr, "Error: shit happened\n");
			return -1;
		}
	}

	return 0;
}

/*
int getNextToken(const char **str, ztoken *token)
{
	const char *first;
	const char *last;
	int res = 0;

	// skip blank characters
	while(isblank(*(*str)) || *(*str) == '\n')
		(*str)++;

	first = last = *str;

	// empty line
	if(!*first)
		return 0;

	// weird character
	if(!isgraph(*first))
	{
		fprintf(stderr, "Error: non printable or non ascii character inside string: (0x%hhX)\n", *first);
		(*str)++;

		return 1;
	}
	// else
	// 	printf("[%c] => ", *first);

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

	// token->position.line = line;
	// token->position.column = column;

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
*/
