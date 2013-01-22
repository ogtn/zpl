//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#ifndef H_ZCOMPILER
#define H_ZCOMPILER


#include "parser.h"


typedef struct zContext
{
	bracketsChecker bChecker;
	char currentLine[LINE_MAX + 1]; // TODO: double buff??
	int currentLineNumber;
} zContext;


#endif
