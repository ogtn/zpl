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


static int isKeyword(ztoken *token)
{
	const ztoken *kw = keywords;

	while(kw->id[0])
	{
		if(strcmp(token->id, kw->id) == 0)
		{
			token->type = kw->type;
			return 1;
		}

		kw++;
	}

	return 0;
}


static int getNextToken(const char **str, ztoken *token)
{
	const char *first;
	const char *last;

	// skip blank characters
	while(isblank(*(*str)))
		(*str)++;

	first = last = *str;

	// empty line
	if(!*first)
		return 0;

	// weird character
	if(!isgraph(*first))
	{
		fprintf(stderr, "Error: non printable or non ascii character outside string: '%c' (0x%x\n", *first, *first);
		return 0;
	}
	
	switch(*first)
	{
		case ';':
			token->type = TOKEN_SEMICOLON;
			token->id[0] = '\0';
			(*str)++;
			break;

		case '=':
			token->type = TOKEN_AFFECT;
			token->id[0] = '\0';
			(*str)++;
			break;

		case '"':
			last++;
			first++; // Bad idea? 
			while(*last != '"' && *last)
				last++;

			snprintf(token->id, last - first + 1, "%s", first);
			token->type = TOKEN_STRING;
			*str = (char *)last + 1;
			break;

		case '/':
			// endline comment
			if(first[1] == '/')
				return 0;
		
		default:
			if(isalpha(*first))
			{
				while(*last == '_' || isalnum(*last))
					last++;

				snprintf(token->id, last - first + 1, "%s", first);

				if(!isKeyword(token))
					token->type = TOKEN_IDENTIFIER;

				*str = (char *)last;
			}
			else if(isdigit(*first))
			{
				while(isdigit(*last))
					last++;

				snprintf(token->id, last - first + 1, "%s", first);

				if(!isKeyword(token))
					token->type = TOKEN_CONSTANT;

				*str = (char *)last;
			}
		break;
	}
	
	if(first == *str)
	{
		(*str)++;
		token->type = TOKEN_UNKNOWN;
		token->id[0] = *first;
		token->id[1] = '\0';
	}

	return *last != '\0';
}


static void checkLine(const char *line, int lineNumber)
{
	if(strlen(line) == LINE_MAX)
	{
		fprintf(stderr, "Line %d exceed %d characters\n", lineNumber, LINE_MAX);
		exit(EXIT_FAILURE);
	}
}


static void cleanLine(const char *line)
{
	char *endl = strchr(line, '\n');

	if(endl != NULL)
		*endl = '\0';
}


static int parseLine(const char *line, int lineNumber)
{
	ztoken token = {"", TOKEN_UNKNOWN};

	printf("l%3d: '%s':\n", lineNumber, line);

	while(getNextToken(&line, &token))
	{
		switch(token.type)
		{
			// cat include/keywords include/types | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_ALIAS:
			case TOKEN_AUTO:
			case TOKEN_BOOL:
			case TOKEN_BREAK:
			case TOKEN_CASE:
			case TOKEN_CLASS:
			case TOKEN_CONST:
			case TOKEN_CONTINUE:
			case TOKEN_CTOR:
			case TOKEN_DEFAULT:
			case TOKEN_DELETE:
			case TOKEN_DO:
			case TOKEN_DOUBLE:
			case TOKEN_DTOR:
			case TOKEN_ELSE:
			case TOKEN_ENUM:
			case TOKEN_FLOAT:
			case TOKEN_FLOAT_16:
			case TOKEN_FLOAT_32:
			case TOKEN_FLOAT_64:
			case TOKEN_FOR:
			case TOKEN_HALF:
			case TOKEN_IF:
			case TOKEN_IMPORT:
			case TOKEN_IN:
			case TOKEN_INLINE:
			case TOKEN_INOUT:
			case TOKEN_INT:
			case TOKEN_MATCH:
			case TOKEN_MODULE:
			case TOKEN_NEW:
			case TOKEN_NODEFAULT:
			case TOKEN_NULL:
			case TOKEN_OUT:
			case TOKEN_PARTIAL:
			case TOKEN_PRIV:
			case TOKEN_PRIVATE:
			case TOKEN_PUB:
			case TOKEN_PUBLIC:
			case TOKEN_RETURN:
			case TOKEN_SIZEOF:
			case TOKEN_STATIC:
			case TOKEN_SWITCH:
			case TOKEN_TEMPLATE:
			case TOKEN_THIS:
			case TOKEN_TYPEDEF:
			case TOKEN_TYPEOF:
			case TOKEN_UBYTE:
			case TOKEN_UCHAR:
			case TOKEN_UCHAR_16:
			case TOKEN_UCHAR_32:
			case TOKEN_UCHAR_8:
			case TOKEN_UINT:
			case TOKEN_UINT_16:
			case TOKEN_UINT_32:
			case TOKEN_UINT_64:
			case TOKEN_UINT_8:
			case TOKEN_ULONG:
			case TOKEN_UNION:
			case TOKEN_USHORT:
			case TOKEN_VOID:
			case TOKEN_WHILE:
				printf("Found keyword: '%s'\n", token.id); break;
			case TOKEN_IDENTIFIER:
				printf("Found identifier '%s'\n", token.id); break;
			case TOKEN_SEMICOLON:
				printf("Found semicolon\n"); break;
			case TOKEN_AFFECT:
				printf("Found affectation\n"); break;
			case TOKEN_STRING:
				printf("Found string: '%s'\n", token.id); break;
			case TOKEN_CONSTANT:
				printf("Found constant: '%s'\n", token.id); break;
			case TOKEN_UNKNOWN: default:
				printf("Found unknown token(%d) id: '%s'\n", token.type, token.id);
				return -1;
		}
	}

	return 0;
}


int main(int argc, char **argv)
{
	char line[LINE_MAX + 1];
	int status = EXIT_SUCCESS;
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
		int lineNumber = 1;
		FILE *f;

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
			
			if(parseLine(line, lineNumber) != 0)
			{
				status = EXIT_FAILURE;
				break;
			}

			lineNumber++;
		}

		if(status == EXIT_SUCCESS)
			printf("##### Parsing of file '%s' completed #####\n\n", fileName);
		else
			printf("##### Parsing of file '%s' Failed #####\n\n", fileName);

		fclose(f);
	}

	return status;
}
