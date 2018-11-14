//
// Created by Subha on 2/5/2018.
//

#ifndef COMPILER_CONSTRUCTION_LEXTEST_H
#define COMPILER_CONSTRUCTION_LEXTEST_H

#include "javaclass.h"
#include "bytecode.h"
#include "init.h"
#include "lexer.h"

#define ERROR_STRING "Invalid Character"

void setendlocation(int loc);
int getendlocation(int index);
int nexttoken();

void match(int token);

void factor();

void term();

void expr();

void stmt();

void opts_stmts();

#endif //COMPILER_CONSTRUCTION_LEXTEST_H
