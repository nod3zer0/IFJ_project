#include "parser-vars.h"
#include "parser-funcs.h"

AST* parseVAR(T_token* token, T_token** lastToken, Stack* symtable, T_BTnode* funtable) {
    T_token* prevToken = token;
    token = getToken();

    // Case $VAR =
    if (token->type == ASSIGN) {

        AST* varTree = ASTInit();


        // if (BTsearch(Top(symtable, erre), prevToken->val)) {
        if (IsInScope(prevToken->val, symtable)) {
            varTree->nodeT = n_asignVar;
        } else {
            varTree->nodeT = n_defvar;
            int er = OK;
            BTinsert(Top(symtable, &er), prevToken->val, NULL);
            if(er!=OK) {exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));}
        }

        varTree->name = prevToken->val;

        token = getToken();

        List* tokenList = malloc(sizeof(List));
        if (tokenList == NULL) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        int err = InitList(tokenList);
        if (err != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        ListAdd(tokenList, token);

        AST* tree = ASTInit();
        if (tree == NULL) {
           
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        tree = GetStringTree(&tokenList, lastToken, symtable);

        // check if its not a function $x = foo()
        if ((*lastToken)->type == ID) {
            varTree->right = parseFunCall(token, symtable, funtable);
            return varTree;
        }

        int er = 0;

        // checks that tree is not empty and doesnt end with semicolon $VAR = ;
        if ((*lastToken)->type != SEMICOLON)
            er = 1;
        else if (tree == NULL)
            er = 1;
        else if (tree->nodeT == n_comp)
            er = 1;
        else if (tree->nodeT == n_undefined)
            er = 1;
        if (er == 1) {
            tree = GetArithmeticTree(tokenList, lastToken, symtable);

            int e = 0;

            if ((*lastToken)->type != SEMICOLON)
                e = 1;
            else if (tree->nodeT == n_comp)
                e = 1;
            else if (tree->nodeT == n_undefined)
                e = 1;
            if (e == 1) {
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
        }

        varTree->right = tree;

        return varTree;
    }
    // Case $VAR ...
    else {
        AST* tree = ASTInit();
        if (tree == NULL) {
           
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        List* tokenList = malloc(sizeof(List));
        if (tokenList == NULL) {
           
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        int err = InitList(tokenList);

        if (err != OK) {
            
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        ListAdd(tokenList, prevToken);
        ListAdd(tokenList, token);

        tree = GetStringTree(&tokenList, lastToken, symtable);

        int er = 0;

        // checks that tree is not empty and doesnt end with semicolon $VAR = ;
        if ((*lastToken)->type != SEMICOLON)
            er = 1;
        else if (tree == NULL)
            er = 1;
        else if (tree->nodeT == n_comp)
            er = 1;

        if (er == 1) {
            tree = GetArithmeticTree(tokenList, lastToken, symtable);

            int e = 0;

            if ((*lastToken)->type != SEMICOLON)
                e = 1;
            else if (tree->nodeT == n_comp)
                e = 1;
            if (e == 1) {
                
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
        }

        return tree;
    }
}

AST* sendToAtree(T_token* token, T_token** lastToken, Stack* symtable) {

    List* tokenList = malloc(sizeof(List));
    if (tokenList == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    int err = InitList(tokenList);
    if (err != OK) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    ListAdd(tokenList, token);

    AST* tree = ASTInit();
    if (tree == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    tree = GetStringTree(&tokenList, lastToken, symtable);

    int er = 0;

    // checks that tree is not empty and doesnt end with semicolon $VAR = ;
    if ((*lastToken)->type != SEMICOLON)
        er = 1;
    else if (tree == NULL)
        er = 1;
    else if (tree->nodeT == n_comp)
        er = 1;

    if (er == 1) {
        tree = GetArithmeticTree(tokenList, lastToken, symtable);

        int e = 0;

        if ((*lastToken)->type != SEMICOLON)
            e = 1;
        else if (tree->nodeT == n_comp)
            e = 1;
        if (e == 1) {
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }
    }

    return tree;
}