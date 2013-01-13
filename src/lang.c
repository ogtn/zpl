//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include <stdlib.h>


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
