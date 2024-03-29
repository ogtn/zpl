//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "zcompiler.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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


static int parseFile(const char *code)
{
	int error_cpt = 0;
	zContext context;
	zToken *tokens;
	int nbTokens;
	zAST *ast;
	int i;

	int res = tokenize(code, &tokens, &nbTokens);
	printf("tokenize() => %s | nbTokens: %d\n", res ? "KO" : "OK", nbTokens);
	initBracketsChecker(&context.bChecker);

	for(i = 0; i < nbTokens; i++)
	{
		switch(tokens[i].type)
		{
			// cat include/keywords | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_ALIAS:
			case TOKEN_AUTO:
			case TOKEN_BREAK:
			case TOKEN_CASE:
			case TOKEN_CLASS:
			case TOKEN_CONST:
			case TOKEN_CONTINUE:
			case TOKEN_CTOR:
			case TOKEN_DEFAULT:
			case TOKEN_DELETE:
			case TOKEN_DO:
			case TOKEN_DTOR:
			case TOKEN_ELSE:
			case TOKEN_ENUM:
			case TOKEN_FOR:
			case TOKEN_IF:
			case TOKEN_IMPORT:
			case TOKEN_IN:
			case TOKEN_INLINE:
			case TOKEN_INOUT:
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
			case TOKEN_UNION:
			case TOKEN_WHILE:
				printf("keyword %s\n", all_keywords[tokens[i].type]); break;

			// cat include/types | sort | sed 's|^[_[:alnum:]]*$|case TOKEN_\U&:|'
			case TOKEN_TYPE:
				printf("type %s\n", all_types[tokens[i].data.type]); break;

			case TOKEN_ASSIGN:
			case TOKEN_COMPARE:
			case TOKEN_DIV:
			case TOKEN_DIV_ASSIGN:
			case TOKEN_ADD:
			case TOKEN_ADD_ASSIGN:
			case TOKEN_INCR:
			case TOKEN_SUB:
			case TOKEN_SUB_ASSIGN:
			case TOKEN_DECR:
			case TOKEN_MULT:
			case TOKEN_MULT_ASSIGN:
			case TOKEN_AND:
			case TOKEN_BIT_AND:
			case TOKEN_BIT_AND_ASSIGN:
			case TOKEN_OR:
			case TOKEN_BIT_OR:
			case TOKEN_BIT_OR_ASSIGN:
			case TOKEN_XOR:
			case TOKEN_XOR_ASSIGN:
			case TOKEN_DOT:
			case TOKEN_ELLIPSE:
			case TOKEN_COMMA:
			case TOKEN_COLON:
			case TOKEN_INF:
			case TOKEN_INF_EQ:
			case TOKEN_LSHIFT:
			case TOKEN_LSHIFT_ASSIGN:
			case TOKEN_SUP:
			case TOKEN_SUP_EQ:
			case TOKEN_RSHIFT:
			case TOKEN_RSHIFT_ASSIGN:
			case TOKEN_MOD:
			case TOKEN_MOD_ASSIGN:
			case TOKEN_BIT_NOT:
			case TOKEN_BIT_NOT_ASSIGN:
			case TOKEN_NOT:
			case TOKEN_NOT_ASSIGN:
			case TOKEN_AT:
			case TOKEN_DOLLAR:
			case TOKEN_SHARP:
			case TOKEN_ESCAPE:
			case TOKEN_BACK_QUOTE:
				printf("operator %c\n", tokens[i].data.char_val); break;

			case TOKEN_OPEN_BLOCK:
			case TOKEN_CLOSE_BLOCK:
			case TOKEN_OPEN_PAREN:
			case TOKEN_CLOSE_PAREN:
			case TOKEN_OPEN_BRACKET:
			case TOKEN_CLOSE_BRACKET:
				checkBracket(&context.bChecker, &tokens[i]); break;

			case TOKEN_COMMENT:
				printf("Comment\n/*%s*/\n", tokens[i].data.str_val); break;

			case TOKEN_IDENTIFIER:
				printf("identifier '%s'\n", tokens[i].data.str_val); break;

			case TOKEN_SEMICOLON:
				printf("semicolon\n"); break;

			case TOKEN_STRING:
				printf("string '%s'\n", tokens[i].data.str_val); break;

			case TOKEN_LITERAL_CHAR:
				printf("char constant %d\n", tokens[i].data.char_val); break;

			case TOKEN_LITERAL_INT:
				printf("int constant %d\n", tokens[i].data.int_val); break;

			case TOKEN_UNKNOWN: default:
				printf("Found unknown token(%d): '%c'\n", tokens[i].type, tokens[i].data.char_val);
				error_cpt++;
		}

		printf("token pos [%d:%d]\n", tokens[i].loc.pos, tokens[i].loc.pos + tokens[i].loc.len);
	}

	checkBracket(&context.bChecker, NULL);
	printf("gbl: %d brkts: %d prnt: %d crlbrkts: %d\n",
		context.bChecker.globalCpt, context.bChecker.bracketCpt,
		context.bChecker.parenCpt, context.bChecker.curlyCpt);

	ast = create_AST(tokens, &tokens[nbTokens]);
	// print_AST(ast);

	// free_AST(ast);
	free(tokens);

	return error_cpt;
}


int main(int argc, char **argv)
{
	char line[LINE_MAX + 1];
	unsigned int codeLen;
	int lineNumber = 1;
	int error_cpt = 0;
	char *code;
	int i;

	// check args
	if(argc <= 1)
	{
		fprintf(stderr, "No source file specified\nUsage: $ lang file1 [file2...files3...]\n");
		exit(EXIT_FAILURE);
	}

	// foreach file
	for(i = 1; i < argc; i++)
	{
		char *fileName = argv[i];
		FILE *f;

		printf("##### Parsing file '%s' #####\n", fileName);

		f = fopen(fileName, "r");

		if(f == NULL)
		{
			fprintf(stderr, "Unable to load file '%s'\n", fileName);
			continue;
		}

		fseek(f, 0, SEEK_END);
		codeLen = ftell(f);
		fseek(f, 0, SEEK_SET);

		code = malloc(codeLen + 1);

		if(code == NULL)
		{
			fprintf(stderr, "Unable to alloc %do of memory for code", codeLen);
			continue;
		}

		fread(code, 1, codeLen, f);
		code[codeLen] = '\0';

		printf("##### CODE ####\n%s\n##### CODE ####\n", code);
		error_cpt += parseFile(code);

		if(error_cpt == 0)
			printf("##### Parsing of file '%s' completed #####\n\n", fileName);
		else
			printf("##### Parsing of file '%s' Failed: %d errors #####\n\n", fileName, error_cpt);

		fclose(f);
	}

	return -!!error_cpt;
}
