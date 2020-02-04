#ifndef __KERNEL_H_INCLUDED__
#define __KERNEL_H_INCLUDED__
#include<environment.h>
#include<cons.h>
#include<kernel.h>

Cell *adder(ENVIRONMENT *bst, Cell *list);
Cell *def(ENVIRONMENT envir, Cell *list);
Cell *is_atom(ENVIRONMENT environment, Cell *list);
#endif
