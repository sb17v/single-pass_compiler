//
// Created by Subha on 2/2/2018.
//
#include "lexer.h"
#include "global.h"

char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;

int lexan() {
    int t;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t');
        else if (t == '\n') {
            lineno = lineno + 1;
        } else if (isdigit(t)) {
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            if (tokenval < 128) {
                return INT8;
            } else if (tokenval >= 128 && tokenval <= 32768) {
                return INT16;
            } else if (tokenval >= 32768 && tokenval <= 2147483647) {
                return INT32;
            }
        } else if (isalpha(t) || t == '_') {
            int p, b = 0;
            while (isalnum(t) || t == '_') {
                lexbuf[b] = t;
                t = getchar();
                b = b + 1;
                if (b >= BSIZE)
                    error("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF)
                ungetc(t, stdin);
            p = lookup(lexbuf);
            if (p == 0) {
                p = insert(lexbuf, ID);
            }
            if (symtable[p].token == IF || symtable[p].token == ELSE || symtable[p].token == WHILE ||
                symtable[p].token == RETURN || symtable[p].token == ARG) {
                tokenval = NONE;
            } else {
                tokenval = (p - 5) + 2;
            }
            return symtable[p].token;
        } else if (t == EOF)
            return DONE;
        else {
            tokenval = NONE;
            return t;
        }
    }
}
