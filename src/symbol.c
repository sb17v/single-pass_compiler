//
// Created by Subha on 2/2/2018.
//
#include <stdio.h>
#include <string.h>
#include "global.h"


char lexemes[STRMAX];
int lastchar = -1;

int lastentry = 0;

int lookup(char s[]) {
    int p;
    for (p = lastentry; p > 0; p = p - 1)
        if (strcmp(symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}

int insert(char s[], int tok) {
    int len;
    len = strlen(s);
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}
