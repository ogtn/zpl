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


#define IDENTIFIER_MAX		64
#define LINE_MAX			1024


typedef enum zType
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
} zType;


extern const char *all_keywords[];
extern const char *all_types[];


#endif // H_LANG
