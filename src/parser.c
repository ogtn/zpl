//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "parser.h"
#include <stdio.h>


void initBracketsChecker(bracketsChecker *bChecker)
{
	bChecker->globalCpt = 0;
	bChecker->bracketCpt = 0;
	bChecker->parenCpt = 0;
	bChecker->curlyCpt = 0;
}


void checkBracket(bracketsChecker *bChecker, ztoken *token)
{
	char last = 0;

	if(bChecker->globalCpt > 0)
		last = bChecker->stack[bChecker->globalCpt - 1];

	if(token == NULL)
	{
		if(bChecker->globalCpt > 0)
		{
			if(bChecker->bracketCpt > 0)
				fprintf(stderr, "checkBracket() failed: missing %d closing ]\n", bChecker->bracketCpt);

			if(bChecker->parenCpt > 0)
				fprintf(stderr, "checkBracket() failed: missing %d closing )\n", bChecker->parenCpt);

			if(bChecker->curlyCpt > 0)
				fprintf(stderr, "checkBracket() failed: missing %d closing }\n", bChecker->curlyCpt);
		}

		return;
	}

	printf("___ %c ___\n", token->data.char_val);

	switch(token->type)
	{
		case TOKEN_OPEN_PAREN:
			bChecker->parenCpt++;
			goto incr;
		case TOKEN_CLOSE_PAREN:
			if(bChecker->globalCpt > 0 && last != '(')
				fprintf(stderr, "checkBracket() failed: closing ] without matching [\n");
			else
				bChecker->parenCpt--;
			goto decr;
		case TOKEN_OPEN_BRACKET:
			bChecker->bracketCpt++;
			goto incr;
		case TOKEN_CLOSE_BRACKET:
			if(bChecker->globalCpt > 0 && last != '[')
				fprintf(stderr, "checkBracket() failed: closing ) without matching (\n");
			else
				bChecker->bracketCpt--;
			goto decr;
		case TOKEN_OPEN_BLOCK:
			if(bChecker->globalCpt > 0 && last != '{')
				fprintf(stderr, "checkBracket() failed: opening { inside %c\n", last);
			else
				bChecker->curlyCpt++;
			goto incr;
		case TOKEN_CLOSE_BLOCK:
			if(bChecker->globalCpt > 0 && last != '{')
				fprintf(stderr, "checkBracket() failed: closing } without matching {\n");
			else
				bChecker->curlyCpt--;
			goto decr;
		default:
			fprintf(stderr, "checkBracket() failed: invalid token: %d\n", token->type);
			return;
	}

	incr:

	if(bChecker->globalCpt >= BRACKETS_MAX)
		fprintf(stderr, "checkBracket() failed: too much nested brackets\n");
	else
		bChecker->stack[bChecker->globalCpt++] = token->data.char_val;
	
	return;

	decr:

	if(bChecker->globalCpt <= 0)
		fprintf(stderr, "checkBracket() failed: closing non opened '%c'\n", token->data.char_val);
	else
		bChecker->globalCpt--;
}
