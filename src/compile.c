//
// Created by Subha on 2/5/2018.
//

#include "compile.h"
#include "global.h"


int lookahead;
int end_locations[127];
int end_loc_index = 0;
struct ClassFile cf;
int index1, index2, index3;
int label1, label2;

void setendlocation(int loc) {
    end_locations[end_loc_index++] = loc;
}

int getendlocation(int index) {
    return end_locations[index];
}

int nexttoken() {
    int token;
    token = lexan();
    if (token == DONE) {
        return -1;
    }
    return token;
}

void match(int token) {
    if (lookahead == token) {
        lookahead = nexttoken();
    } else {
        error(ERROR_STRING);
    }
}

void factor() {
    if (lookahead == '(') {
        match('(');
        expr();
        match(')');
    } else if (lookahead == '-') {
        factor();
        emit(isub);
    } else if (lookahead == INT8) {
        emit2(bipush, tokenval);
        match(INT8);
    } else if (lookahead == INT16) {
        emit3(sipush, tokenval);
        match(INT16);
    } else if (lookahead == INT32) {
        emit2(ldc, constant_pool_add_Integer(&cf, tokenval));
        match(INT32);
    } else if (lookahead == ID) {
        emit2(iload, tokenval);
        match(ID);
    } else if (lookahead == ARG) {
        match(ARG);
        match('[');
        int tok = tokenval;
        match(INT8);
        match(']');
        emit(aload_1);
        emit2(bipush, tok);
        emit(iaload);
    } else {
        error(ERROR_STRING);
    }
}

void term() {
    factor();
    while (1) {
        if (lookahead == '*') {
            match('*');
            factor();
            emit(imul);
        } else if (lookahead == '/') {
            match('/');
            factor();
            emit(idiv);
        } else if (lookahead == '%') {
            match('%');
            factor();
            emit(irem);
        } else {
            break;
        }
    }
}

void expr() {
    term();
    while (1) {
        if (lookahead == '+') {
            match('+');
            term();
            emit(iadd);
        } else if (lookahead == '-') {
            match('-');
            term();
            emit(isub);
        } else {
            break;
        }
    }

}

void stmt() {
    if (lookahead == '{') {
        match('{');
        opts_stmts();
        match('}');
    } else if (lookahead == ID) {
        int var = tokenval;
        match(ID);
        match('=');
        expr();
        emit2(istore, var);
        match(';');
    } else if (lookahead == IF) {
        match(IF);
        match('(');
        expr();
        match(')');
        emit(iconst_0);
        int loc = pc;
        emit3(if_icmpeq, 0);
        stmt();
        backpatch(loc, pc - loc);
        match(ELSE);
        stmt();
    } else if (lookahead == WHILE) {
        match(WHILE);
        match('(');
        int test = pc;
        expr();
        match(')');
        emit(iconst_0);
        int loc = pc;
        emit3(if_icmpeq, 0);
        stmt();
        emit3(goto_, test - pc);
        backpatch(loc, pc - loc);
    } else if (lookahead == RETURN) {
        match(RETURN);
        expr();
        emit(istore_2);
        setendlocation(pc);
        emit3(goto_, PAD);
        match(';');
    } else {
        error(ERROR_STRING);
    }
}

void opts_stmts() {
    if (lookahead == '{' || lookahead == ID || lookahead == IF || lookahead == WHILE || lookahead == RETURN) {
        stmt();
        opts_stmts();
    } else {
        return;
    }
}

void init_byte_emitter() {
    // set up new class file structure
    init_ClassFile(&cf);

    // class has public access
    cf.access = ACC_PUBLIC;

    // class name is "Calc"
    cf.name = "Code";

    // no fields
    cf.field_count = 0;

    // one method
    cf.method_count = 1;

    // allocate array of methods (just one "main" in our example)
    cf.methods = (struct MethodInfo *) malloc(cf.method_count * sizeof(struct MethodInfo));

    // method has public access and is static
    cf.methods[0].access = ACC_PUBLIC | ACC_STATIC;

    // method name is "main"
    cf.methods[0].name = "main";

    // method descriptor of "void main(String[] arg)"
    cf.methods[0].descriptor = "([Ljava/lang/String;)V";

    // max operand stack size of this method
    cf.methods[0].max_stack = 127;

    // the number of local variables in the local variable array
    //   local variable 0 contains "arg"
    //   local variable 1 contains "val"
    //   local variable 2 contains "i" and "result"
    cf.methods[0].max_locals = 127;

    // set up new bytecode buffer
    init_code();

    // generate code
/*LOC*/
/*000*/    emit(aload_0);
/*001*/    emit(arraylength);        // arg.length
/*002*/    emit2(newarray, T_INT);
/*004*/    emit(astore_1);            // val = new int[arg.length]
/*005*/    emit(iconst_0);
/*006*/    emit(istore_2);            // i = 0
    label1 = pc;            // label1:
/*007*/    emit(iload_2);
/*008*/    emit(aload_0);
/*009*/    emit(arraylength);
    label2 = pc;
/*010*/    emit3(if_icmpge, PAD);        // if i >= arg.length then goto label2
/*013*/    emit(aload_1);
/*014*/    emit(iload_2);
/*015*/    emit(aload_0);
/*016*/    emit(iload_2);
/*017*/    emit(aaload);            // push arg[i] parameter for parseInt
    index1 = constant_pool_add_Methodref(&cf, "java/lang/Integer", "parseInt", "(Ljava/lang/String;)I");
/*018*/    emit3(invokestatic, index1);    // invoke Integer.parseInt(arg[i])
/*021*/    emit(iastore);            // val[i] = Integer.parseInt(arg[i])
/*022*/    emit32(iinc, 2, 1);        // i++
/*025*/    emit3(goto_, label1 - pc);    // goto label1
    backpatch(label2, pc - label2);    // label2:
}

void end_byte_emitter() {
    index2 = constant_pool_add_Fieldref(&cf, "java/lang/System", "out", "Ljava/io/PrintStream;");
/*036*/    emit3(getstatic, index2);    // get static field System.out of type PrintStream
/*039*/    emit(iload_2);            // push parameter for println()
    index3 = constant_pool_add_Methodref(&cf, "java/io/PrintStream", "println", "(I)V");
/*040*/    emit3(invokevirtual, index3);    // invoke System.out.println(result)
/*043*/    emit(return_);            // return

    // length of bytecode is in the emmiter's pc variable
    cf.methods[0].code_length = pc;

    // must copy code to make it persistent
    cf.methods[0].code = copy_code();

    // save class file to "Code.class"
    save_classFile(&cf);
}

int main() {
    /* Loading keywords into symbol table */
    init();

    /* Invoking lexer for first time */
    lookahead = lexan();

    /*Initial part of the byte emitter*/
    init_byte_emitter();

    /*Start of the parser*/
    stmt();

    /*Backpatching for return*/
    int i;
    for (i = 0; i < end_loc_index; i++) {
        int endlocation = getendlocation(i);
        backpatch(endlocation, pc - endlocation);
    }

    /*End part of byte emitter*/
    end_byte_emitter();
    return 0;

}
