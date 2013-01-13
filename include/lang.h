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


#define LINE_MAX			1024
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
	TOKEN_COMPARE,
	TOKEN_DIV,
	TOKEN_ADD,
	TOKEN_ASSIGN,
	TOKEN_MINUS,
	TOKEN_MULT,
	TOKEN_AND,
	TOKEN_BIT_AND,
	TOKEN_OR,
	TOKEN_BIT_OR,
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


typedef enum ztype
{
	// cat include/types | sort | sed 's|^[_[:alnum:]]*$|TYPE_\U&,|'
	TYPE_BOOL,
	TYPE_BYTE,
	TYPE_CHAR,
	TYPE_CHAR_16,
	TYPE_CHAR_32,
	TYPE_CHAR_8,
	TYPE_DOUBLE,
	TYPE_FLOAT,
	TYPE_FLOAT_16,
	TYPE_FLOAT_32,
	TYPE_FLOAT_64,
	TYPE_HALF,
	TYPE_INT,
	TYPE_INT_16,
	TYPE_INT_32,
	TYPE_INT_64,
	TYPE_INT_8,
	TYPE_LONG,
	TYPE_SHORT,
	TYPE_UBYTE,
	TYPE_UCHAR,
	TYPE_UCHAR_16,
	TYPE_UCHAR_32,
	TYPE_UCHAR_8,
	TYPE_UINT,
	TYPE_UINT_16,
	TYPE_UINT_32,
	TYPE_UINT_64,
	TYPE_UINT_8,
	TYPE_ULONG,
	TYPE_USHORT,
	TYPE_VOID
} ztype;


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


const char *all_keywords[] =
{
	// sort include/keywords | sed 's|^[_[:alnum:]]*$|"&",|'
	"alias",
	"auto",
	"break",
	"case",
	"class",
	"const",
	"continue",
	"ctor",
	"default",
	"delete",
	"do",
	"dtor",
	"else",
	"enum",
	"for",
	"if",
	"import",
	"in",
	"inline",
	"inout",
	"match",
	"module",
	"new",
	"nodefault",
	"null",
	"out",
	"partial",
	"priv",
	"private",
	"pub",
	"public",
	"return",
	"sizeof",
	"static",
	"switch",
	"template",
	"this",
	"typedef",
	"typeof",
	"union",
	"while",
	NULL
};


const char *all_types[] =
{
	// sort include/types | sed 's|^[_[:alnum:]]*$|"&",|'
	"bool",
	"byte",
	"char",
	"char_16",
	"char_32",
	"char_8",
	"double",
	"float",
	"float_16",
	"float_32",
	"float_64",
	"half",
	"int",
	"int_16",
	"int_32",
	"int_64",
	"int_8",
	"long",
	"short",
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
	"ushort",
	"void",
	NULL
};


#endif // H_LANG
