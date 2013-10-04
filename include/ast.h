//  lang: ridiculous language
//
//  Copyright: (c) 2013 Olivier Guittonneau <OpenMEngine@gmail.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the Do What The Fuck You Want To
//  Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/projects/COPYING.WTFPL for more details.


#ifndef H_AST
#define H_AST


#include "tokenizer.h"


typedef enum zNodeType
{
    NODE_TYPE_TRANSLATION_UNIT,
    NODE_TYPE_VAR_DECL,
    NODE_TYPE_ASSIGNMENT,
    NODE_TYPE_CALL,
    NODE_TYPE_SELECTION,
    NODE_TYPE_BINARY_OPERATOR,
} zNodeType;


typedef struct zTranslationUnit zTranslationUnit;
typedef struct zVarDecl zVarDecl;
typedef struct zAssign zAssign;
typedef struct zCall zCall;
typedef struct zSelection zSelection;
typedef struct zBinaryOperator zBinaryOperator;
typedef struct zVarRef zVarRef;
typedef struct zLiteral zLiteral;
typedef struct zStatement zStatement;
typedef struct zExpression zExpression;


struct zSelection
{
    int nbCond;
    zExpression *cond;
    zStatement *bodies;
};


struct zBinaryOperator
{
    zType type;
    zToken *operator;
    zExpression *leftExpression;
    zExpression *rightExpression;
};


struct zExpression
{
    zType type;
    zVarRef *varRef;
};


struct zVarRef
{
    zType type;
    zToken *token;
};


struct zLiteral
{
    zType type;
    zToken *token;
};


struct zStatement
{
    zVarDecl *varDecl;
    // zExpression *expression;
};


struct zVarDecl
{
    zToken *type;
    zToken *id;
};


struct zTranslationUnit
{
    zVarDecl *varDecl;
};

/* typedef zNode
{
    zNodeType type;

    union
    {
        zTranslationUnit *translationUnit;
        zVarDecl *varDecl;
        zAssign *assignment;
        zCall *call;
        zSelection *selection;
    } node;
} zNode; */


typedef struct zAST
{
    zTranslationUnit *translationUnit;
} zAST;


zAST *create_AST(zToken *tokens, zToken *end);
zTranslationUnit *create_TranslationUnitAST(zToken *tokens, zToken *end);
zVarDecl *create_VarDecl(zToken *tokens, zToken *end);
void print_AST(zAST *ast);
void free_AST(zAST *ast);


#endif // guard
