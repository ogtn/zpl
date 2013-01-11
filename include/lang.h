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


typedef enum tokenType
{
	// cat include/keywords include/types | sort | sed 's|^[_[:alnum:]]*$|TOKEN_\U&,|'
	TOKEN_ALIAS,
	TOKEN_AUTO,
	TOKEN_BOOL,
	TOKEN_BREAK,
	TOKEN_CASE,
	TOKEN_CLASS,
	TOKEN_CONST,
	TOKEN_CONTINUE,
	TOKEN_CTOR,
	TOKEN_DEFAULT,
	TOKEN_DELETE,
	TOKEN_DO,
	TOKEN_DOUBLE,
	TOKEN_DTOR,
	TOKEN_ELSE,
	TOKEN_ENUM,
	TOKEN_FLOAT,
	TOKEN_FLOAT_16,
	TOKEN_FLOAT_32,
	TOKEN_FLOAT_64,
	TOKEN_FOR,
	TOKEN_HALF,
	TOKEN_IF,
	TOKEN_IMPORT,
	TOKEN_IN,
	TOKEN_INLINE,
	TOKEN_INOUT,
	TOKEN_INT,
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
	TOKEN_UBYTE,
	TOKEN_UCHAR,
	TOKEN_UCHAR_16,
	TOKEN_UCHAR_32,
	TOKEN_UCHAR_8,
	TOKEN_UINT,
	TOKEN_UINT_16,
	TOKEN_UINT_32,
	TOKEN_UINT_64,
	TOKEN_UINT_8,
	TOKEN_ULONG,
	TOKEN_UNION,
	TOKEN_USHORT,
	TOKEN_VOID,
	TOKEN_WHILE,

	// sort include/special_chars | sed 's|\([_a-z]*\) \(.*\)|TOKEN_\U\1,|'
/*	TOKEN_AFFECT,
	TOKEN_BEGIN_COMMENT,
	TOKEN_COMPARE,
	TOKEN_DIV,
	TOKEN_DOT,
	TOKEN_ELLIPSE,
	TOKEN_ENDLINE_COMMENT,
*/

	// do not remove
	TOKEN_SEMICOLON,
	TOKEN_IDENTIFIER,
	TOKEN_AFFECT,
	TOKEN_STRING,
	TOKEN_CONSTANT,

	TOKEN_UNKNOWN
} tokenType;


typedef struct ztoken
{
	char id[IDENTIFIER_MAX];
	tokenType type;
} ztoken;


const ztoken keywords[] =
{
	// sort include/keywords | sed 's|^[_[:alnum:]]*$|{"&", TOKEN_\U&},|'
	{"alias", TOKEN_ALIAS},
	{"auto", TOKEN_AUTO},
	{"break", TOKEN_BREAK},
	{"case", TOKEN_CASE},
	{"class", TOKEN_CLASS},
	{"const", TOKEN_CONST},
	{"continue", TOKEN_CONTINUE},
	{"ctor", TOKEN_CTOR},
	{"default", TOKEN_DEFAULT},
	{"delete", TOKEN_DELETE},
	{"do", TOKEN_DO},
	{"dtor", TOKEN_DTOR},
	{"else", TOKEN_ELSE},
	{"enum", TOKEN_ENUM},
	{"for", TOKEN_FOR},
	{"if", TOKEN_IF},
	{"import", TOKEN_IMPORT},
	{"in", TOKEN_IN},
	{"inline", TOKEN_INLINE},
	{"inout", TOKEN_INOUT},
	{"match", TOKEN_MATCH},
	{"module", TOKEN_MODULE},
	{"new", TOKEN_NEW},
	{"nodefault", TOKEN_NODEFAULT},
	{"null", TOKEN_NULL},
	{"out", TOKEN_OUT},
	{"partial", TOKEN_PARTIAL},
	{"priv", TOKEN_PRIV},
	{"private", TOKEN_PRIVATE},
	{"pub", TOKEN_PUB},
	{"public", TOKEN_PUBLIC},
	{"return", TOKEN_RETURN},
	{"sizeof", TOKEN_SIZEOF},
	{"static", TOKEN_STATIC},
	{"switch", TOKEN_SWITCH},
	{"template", TOKEN_TEMPLATE},
	{"this", TOKEN_THIS},
	{"typedef", TOKEN_TYPEDEF},
	{"typeof", TOKEN_TYPEOF},
	{"union", TOKEN_UNION},
	{"while", TOKEN_WHILE},

	// Native types
	// sort include/types | sed 's|^[_[:alnum:]]*$|{"&", TOKEN_\U&},|'
	{"bool", TOKEN_BOOL},
	{"double", TOKEN_DOUBLE},
	{"float", TOKEN_FLOAT},
	{"float", TOKEN_FLOAT},
	{"float_16", TOKEN_FLOAT_16},
	{"float_32", TOKEN_FLOAT_32},
	{"float_64", TOKEN_FLOAT_64},
	{"half", TOKEN_HALF},
	{"int", TOKEN_INT},
	{"ubyte", TOKEN_UBYTE},
	{"uchar", TOKEN_UCHAR},
	{"uchar_16", TOKEN_UCHAR_16},
	{"uchar_32", TOKEN_UCHAR_32},
	{"uchar_8", TOKEN_UCHAR_8},
	{"uint", TOKEN_UINT},
	{"uint_16", TOKEN_UINT_16},
	{"uint_32", TOKEN_UINT_32},
	{"uint_64", TOKEN_UINT_64},
	{"uint_8", TOKEN_UINT_8},
	{"ulong", TOKEN_ULONG},
	{"ushort", TOKEN_USHORT},
	{"void", TOKEN_VOID},

	// do not remove
	{"", TOKEN_UNKNOWN}
};


#endif // H_LANG
