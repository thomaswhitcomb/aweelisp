#ifndef __NATIVE_H_INCLUDED__
#define __NATIVE_H_INCLUDED__
#include<environment.h>
#include<cell.h>
#include<eval.h>
#include<native.h>
void native_initialize();
Cell* native_fetch(char *name);
int native_eval_params(LAMBDA lambda);
LAMBDA native_ptr(LAMBDA lambda);
#endif
