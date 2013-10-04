//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#ifndef H_PARSER
#define H_PARSER


#include "tokenizer.h"


#define BRACKETS_MAX		64


typedef struct bracketsChecker
{
	char stack[BRACKETS_MAX];
	int globalCpt;
	int bracketCpt;
	int parenCpt;
	int curlyCpt;
} bracketsChecker;


void initBracketsChecker(bracketsChecker *bChecker);
void checkBracket(bracketsChecker *bChecker, zToken *token);


#endif // guard
