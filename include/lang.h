//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#ifndef H_LANG
#define H_LANG


#include <stdlib.h>


#define LINE_MAX			512
#define IDENTIFIER_MAX		64


const char * keywords[] =
{
	"alias",
	"auto",
	"bool",
	"break",
	"case",
	"class",
	"const",
	"continue",
	"ctor",
	"default",
	"do",
	"double",
	"dtor",
	"enum",
	"float",
	"float",
	"float_16",
	"float_32",
	"float_64",
	"for",
	"half",
	"import",
	"in",
	"inline",
	"inout",
	"int",
	"match",
	"module",
	"nodefault",
	"null",
	"out",
	"priv",
	"private",
	"pub",
	"public",
	"return",
	"sizeof",
	"static",
	"switch",
	"template",
	"typedef",
	"typeof",
	"ubyte",
	"uchar",
	"uchar_16",
	"uchar_32",
	"uchar_8",
	"uint",
	"uint_16",
	"uint_32",
	"uint_64",
	"uint_8",
	"ulong",
	"union",
	"ushort",
	"void",
	"while",
	NULL
};


typedef enum tokenType
{
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_MULT,
	TOKEN_DIV,
	TOKEN_OPEN_PARENT,
	TOKEN_CLOSE_PARENT,
	TOKEN_OPEN_BRACE,
	TOKEN_CLOSE_BRACE,
	TOKEN_OPEN_CURLY_BRACE,
	TOKEN_CLOSE_CULRY_BRACE,
	TOKN_COMMA,
	TOKEN_DOT,
	TOKEN_SEMICOLON,
	TOKEN_IDENTIFIER,
	TOKEN_TYPE,
	TOKEN_AFFECTATION,
	TOKEN_INT_VAL
} tokenType;


typedef struct token
{
	tokenType type;
	char info[IDENTIFIER_MAX];
} token;


#endif // H_LANG
