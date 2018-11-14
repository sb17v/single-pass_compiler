//
// Created by Subhadeep on 2/7/18.
//

#include "lextest.h"
#include "global.h"

char *gettoken(int token) {
    switch (token) {
        case IF:
            return "IF";
        case ELSE:
            return "ELSE";
        case WHILE:
            return "WHILE";
        case RETURN:
            return "RETURN";
        case ARG:
            return "ARG";
        case DONE:
            return "DONE";
        case ID :
            return "ID";
        case INT8:
            return "INT8";
        case INT16:
            return "INT16";
        case INT32:
            return "INT32";
        default:
            return "UNKNOWN_TOKEN";
    }
}

int main() {
    int token;
    init();
    while (1) {
        token = lexan();
        if(token == DONE){
	    break;
	} else { 
	    if (token <= 255) {
            	printf("<%c, %d>", token, tokenval);
            } else {
            	printf("<%s, %d>", gettoken(token), tokenval);
            }
        }	
    }
    printf("\n");
    return 0;
}
