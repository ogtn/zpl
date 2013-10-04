//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#include "ast.h"
#include <stdlib.h>
#include <stdio.h>


zAST *create_AST(zToken *tokens, zToken *end)
{
    zAST *ast;

    ast = malloc(sizeof(zAST));
    ast->translationUnit = create_TranslationUnitAST(tokens, end);

    return ast;
}


void print_AST(zAST *ast)
{

}


void free_AST(zAST *ast)
{
    free(ast); // major leak :D
}


zTranslationUnit *create_TranslationUnitAST(zToken *tokens, zToken *end)
{
    zTranslationUnit *tu = NULL;

    if((&tokens[2] < end))
    {
        tu = malloc(sizeof(zTranslationUnit));
        tu->varDecl = create_VarDecl(tokens, end);
    }

    return tu;
}


zVarDecl *create_VarDecl(zToken *tokens, zToken *end)
{
    zVarDecl *vd = NULL;

    if((tokens[0].type == TOKEN_TYPE) &&
        (tokens[1].type == TOKEN_IDENTIFIER) &&
        (tokens[2].type == TOKEN_SEMICOLON))
    {
        vd = malloc(sizeof(zVarDecl));
        vd->type = &tokens[0];
        vd->id = &tokens[1];

        printf("Declaration of var '%s' typed '%d'\n",
            vd->type->data.str_val,
            vd->id->data.type);
    }

    return vd;
}



