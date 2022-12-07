/*****************************************************************
 * \file   parser.c
 * \brief
 *
 * \author Matej Smida xsmida06
 * \date   December 2022
 *********************************************************************/

#include "parser.h"
#include "parser-cond.h"
#include "parser-funcs.h"
#include "parser-vars.h"

AST* PROLOG() {
    if (detectProlog() != 1) exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));

    AST* tree = PROGRAM();

    return tree;
}

AST* PROGRAM() {
    AST* tree = ASTInit();

    if (tree == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    tree->nodeT = n_stList;

    T_BTnode* sym;
    BTinit(&sym);

    T_BTnode* funtable;
    BTinit(&funtable);

    Stack s;
    Stack* stack = &s;
    int err = InitStack(stack);

    if (err != OK) {
        ASTDtor(tree, true);
        BTdispose(sym, freeBtree);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    Push(stack, &sym);

    AST* root = tree;

    T_token* token;
    token = getToken();

    while (token->type != TOKEN_EOF) {

        if (strcmp(token->val->chars, "function") == 0) {
            T_BTnode* funSym;
            BTinit(&funSym);

            Stack fs;
            Stack* funStack = &fs;
            int e = InitStack(funStack);

            if (e != OK) {
                freeAll(tree, token, stack);
                BTdispose(sym, freeBtree);
                exit(WriteErrorMessage(err));
            }

            Push(funStack, &funSym);

            tree->left = parseFunction(token, funStack, funtable);
        } else if (token->type == EPILOG) {
            int err = parseEpilog(token);

            if (err != OK) {
                freeAll(tree, token, stack);
                BTdispose(sym, freeBtree);
                exit(WriteErrorMessage(err));
            }
        } else {
            tree->left = BODYCASE(token, stack, funtable);
        }

        token = getToken();

        if (token->type == TOKEN_EOF) break;

        tree->right = ASTInit();
        tree->right->nodeT = n_stList;
        tree = tree->right;
    }
    return root;
}

AST* BODY(T_token* token, Stack* symtable, T_BTnode* funtable) {

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    if (token->type == NOT_TOKEN) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    AST* root = tree;

    while (token->type != TOKEN_EOF) {

        token = getToken();
        tree->left = BODYCASE(token, symtable, funtable);
        tree->right = ASTInit();
        tree->right->nodeT = n_stList;
        tree = tree->right;
    }

    return root;
}

AST* BODYCASE(T_token* token, Stack* symtable, T_BTnode* funtable) {

    switch (token->type) {
        case KEYWORD:
            if (strcmp(token->val->chars, "if") == 0) {
                return parseIf(token, symtable, funtable);
                break;
            } else if (strcmp(token->val->chars, "while") == 0) {
                return parseWhile(token, symtable, funtable);
                break;
            } else {

                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;

        case VAR: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return parseVAR(token, &lastToken, symtable, funtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case ID:
            return parseFunCall(token, symtable, funtable);
            break;

        case INT: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case FLOAT: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case FLOAT_DEC_EXP: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case FLOAT_EXP: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case NOT_TOKEN:

            exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            break;

        default: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            return sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }
    }
}

int parseEpilog(T_token* token) {
    token = getToken();

    if (token->type != TOKEN_EOF) return SYNTACTIC_ANALYSIS_ERROR;

    return OK;
}

void freeAll(AST* tree, T_token* token, Stack* stack) {
    ASTDtor(tree, true);
    tokenDtor(token);
    DisposeStack(stack, freeStack);

    return;
}
// TODO free stack and its content
void freeStack() {
    return;
}

void freeList() {
    return;
}

void freeBtree() {
    return;
}