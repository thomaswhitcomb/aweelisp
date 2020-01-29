#ifndef __EVAL_H_INCLUDED__   
#define __EVAL_H_INCLUDED__
#include <bst.h>
#include <cons.h>

typedef Cell * (*LAMBDA)(BST *bst, Cell *cons);
//P R O T O T Y P E S
Cell *eval(BST *environment,Cell *cons);
#endif
