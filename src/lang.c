//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int isKeyword(const char *word)
{
	int i = 0;

	while(keywords[i])
	{
		if(strcmp(word, keywords[i]) == 0)
			break;

		i++;
	}

	return keywords[i] == NULL ? -1 : i;
}


int getNextToken(char **str, token *tok)
{
	const char *first;
	const char *last;
	size_t len;

	// skip blank characters
	while(isblank(*(*str)))
	{
		//printf("skiping '%c'\n", *(*str));
		(*str)++;
	}

	first = last = *str;

	if(isalnum(*first))
	{
		while(isalnum(*last))
			last++;

		snprintf(tok->info, last - first + 1, "%s", first);
		//printf("Found word: '%s'\n", tok->info);

		if(isKeyword(tok->info) == -1)
			tok->type = TOKEN_IDENTIFIER;
		else
			tok->type = -1;

		*str = last;

		return 1;
	}
	else if(*first == ';')
	{
		tok->type = TOKEN_SEMICOLON;
		tok->info[0] = '\0';

		(*str)++;

		return 1;
	}
	else
		return 0;
}


void checkLine(const char *line, int lineNumber)
{
	if(strlen(line) == LINE_MAX)
	{
		fprintf(stderr, "Line %d exceed %d characters\n", lineNumber, LINE_MAX);
		exit(EXIT_FAILURE);
	}
}


void cleanLine(const char *line)
{
	char *endl = strchr(line, '\n');

	if(endl != NULL)
		*endl = '\0';
}


void parseLine(const char *line, int lineNumber)
{
	token tok;

	printf("l%3d: '%s':\n", lineNumber, line);

	while(getNextToken(&line, &tok))
	{
		switch(tok.type)
		{
			case TOKEN_IDENTIFIER:
				printf("\tFound identifier '%s'\n", tok.info); break;
			case TOKEN_TYPE:
				printf("\tFound type '%s'\n", tok.info); break;
			case TOKEN_SEMICOLON:
				printf("\tFound semicolon\n"); break;
			default:
				printf("\tFound unknown token(%d) info: '%s'\n", tok.type, tok.info); break;
		}
	}
}


int main(int argc, char **argv)
{
	char line[LINE_MAX + 1];
	int lineNumber = 1;
	FILE *f;
	int i;

	// check args
	if(argc <= 1)
	{
		fprintf(stderr, "No source file specified\nlang files\n");
		exit(EXIT_FAILURE);
	}

	// foreach file
	for(i = 1; i < argc; i++)
	{
		char *fileName = argv[i];

		// opening source file
		f = fopen(fileName, "r");

		if(f == NULL)
		{
			fprintf(stderr, "Unable to load file '%s'\n", fileName);
			continue;
		}

		printf("##### Parsing file '%s' #####\n", fileName);

		while(fgets(line, LINE_MAX + 1, f))
		{
			checkLine(line, lineNumber);
			cleanLine(line);
			parseLine(line, lineNumber);

			lineNumber++;
		}

		printf("##### Parsing of file '%s' completed #####\n\n", fileName);
		fclose(f);
	}

	return EXIT_SUCCESS;
}
