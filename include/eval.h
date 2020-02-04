#ifndef __EVAL_H_INCLUDED__   
#define __EVAL_H_INCLUDED__
#include <bst.h>
#include <cons.h>
#include <environment.h>

typedef Cell * (*LAMBDA)(ENVIRONMENT envir, Cell *cons);
//P R O T O T Y P E S
Cell *eval(ENVIRONMENT environment,Cell *cons);
Cell *eval_main(char* str);
#endif
