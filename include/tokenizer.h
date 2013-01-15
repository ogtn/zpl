//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#ifndef H_TOKENIZER
#define H_TOKENIZER


#include "lang.h"


#define IDENTIFIER_MAX		64


typedef enum tokenType
{
	// keywords
	// cat include/keywords | sort | sed 's|^[_[:alnum:]]*$|TOKEN_\U&,|'
	TOKEN_ALIAS,
	TOKEN_AUTO,
	TOKEN_BREAK,
	TOKEN_CASE,
	TOKEN_CLASS,
	TOKEN_CONST,
	TOKEN_CONTINUE,
	TOKEN_CTOR,
	TOKEN_DEFAULT,
	TOKEN_DELETE,
	TOKEN_DO,
	TOKEN_DTOR,
	TOKEN_ELSE,
	TOKEN_ENUM,
	TOKEN_FOR,
	TOKEN_IF,
	TOKEN_IMPORT,
	TOKEN_IN,
	TOKEN_INLINE,
	TOKEN_INOUT,
	TOKEN_MATCH,
	TOKEN_MODULE,
	TOKEN_NEW,
	TOKEN_NODEFAULT,
	TOKEN_NULL,
	TOKEN_OUT,
	TOKEN_PARTIAL,
	TOKEN_PRIV,
	TOKEN_PRIVATE,
	TOKEN_PUB,
	TOKEN_PUBLIC,
	TOKEN_RETURN,
	TOKEN_SIZEOF,
	TOKEN_STATIC,
	TOKEN_SWITCH,
	TOKEN_TEMPLATE,
	TOKEN_THIS,
	TOKEN_TYPEDEF,
	TOKEN_TYPEOF,
	TOKEN_UNION,
	TOKEN_WHILE,

	// operators
	TOKEN_ASSIGN,	
	TOKEN_COMPARE,
	TOKEN_DIV,
	TOKEN_DIV_ASSIGN,
	TOKEN_ADD,
	TOKEN_ADD_ASSIGN,
	TOKEN_INCR,
	TOKEN_SUB,
	TOKEN_SUB_ASSIGN,
	TOKEN_DECR,
	TOKEN_MULT,
	TOKEN_MULT_ASSIGN,
	TOKEN_AND,
	TOKEN_BIT_AND,
	TOKEN_BIT_AND_ASSIGN,
	TOKEN_OR,
	TOKEN_BIT_OR,
	TOKEN_BIT_OR_ASSIGN,
	TOKEN_XOR,
	TOKEN_DOT,
	TOKEN_COMMA,

	// other special cases
	TOKEN_TYPE,
	TOKEN_IDENTIFIER,
	TOKEN_SEMICOLON,
	TOKEN_STRING,
	TOKEN_LITERAL_CHAR,
	TOKEN_LITERAL_INT,
	TOKEN_COLON,
	TOKEN_INF,
	TOKEN_SUP,
	TOKEN_MOD,
	TOKEN_TILDE,
	TOKEN_NOT,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_OPEN_BLOCK,
	TOKEN_CLOSE_BLOCK,
	TOKEN_OPEN_BRACKET,
	TOKEN_CLOSE_BRACKET,
	TOKEN_AT,
	TOKEN_DOLLAR,
	TOKEN_SHARP,
	TOKEN_ESCAPE,
	TOKEN_BACK_QUOTE,

	TOKEN_UNKNOWN
} tokenType;


typedef struct ztoken
{
	tokenType type;

	union
	{
		char char_val;
		int int_val;
		float float_val;
		char *str_val;
		ztype type;
	} data;
} ztoken;


int getNextToken(const char **str, ztoken *token);


#endif // H_TOKENIZER
