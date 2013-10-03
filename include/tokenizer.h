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
	TOKEN_ADD,
	TOKEN_ADD_ASSIGN,
	TOKEN_AND,
	TOKEN_ASSIGN,
	TOKEN_BIT_AND,
	TOKEN_BIT_AND_ASSIGN,
	TOKEN_BIT_NOT,
	TOKEN_BIT_NOT_ASSIGN,
	TOKEN_BIT_OR,
	TOKEN_BIT_OR_ASSIGN,
	TOKEN_COMMA,
	TOKEN_COMPARE,
	TOKEN_DECR,
	TOKEN_DIV,
	TOKEN_DIV_ASSIGN,
	TOKEN_DOT,
	TOKEN_ELLIPSE,
	TOKEN_INCR,
	TOKEN_INF,
	TOKEN_INF_EQ,
	TOKEN_LSHIFT,
	TOKEN_LSHIFT_ASSIGN,
	TOKEN_MOD,
	TOKEN_MOD_ASSIGN,
	TOKEN_MULT,
	TOKEN_MULT_ASSIGN,
	TOKEN_NOT,
	TOKEN_NOT_ASSIGN,
	TOKEN_OR,
	TOKEN_RSHIFT,
	TOKEN_RSHIFT_ASSIGN,
	TOKEN_SUB,
	TOKEN_SUB_ASSIGN,
	TOKEN_SUP,
	TOKEN_SUP_EQ,
	TOKEN_XOR,
	TOKEN_XOR_ASSIGN,

	// other special cases
	TOKEN_AT,
	TOKEN_BACK_QUOTE,
	TOKEN_CLOSE_BLOCK,
	TOKEN_CLOSE_BRACKET,
	TOKEN_CLOSE_PAREN,
	TOKEN_COLON,
	TOKEN_COMMENT,
	TOKEN_DOLLAR,
	TOKEN_ESCAPE,
	TOKEN_IDENTIFIER,
	TOKEN_LITERAL_CHAR,
	TOKEN_LITERAL_INT,
	TOKEN_OPEN_BLOCK,
	TOKEN_OPEN_BRACKET,
	TOKEN_OPEN_PAREN,
	TOKEN_SEMICOLON,
	TOKEN_SHARP,
	TOKEN_STRING,
	TOKEN_TYPE,

	TOKEN_UNKNOWN
} tokenType;



typedef struct position
{
	unsigned short line;
	unsigned short column;
} position;


typedef struct ztoken
{
	tokenType type;
	position pos;

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
