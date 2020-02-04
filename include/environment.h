#ifndef __ENVIRONMENT_H_INCLUDED__   
#define __ENVIRONMENT_H_INCLUDED__
#include <bst.h>

#define stack_max  10

struct environment{
    struct environment *parent;
    BST *bst;
};
typedef struct environment *ENVIRONMENT;

ENVIRONMENT environment_new(ENVIRONMENT parent);
ENVIRONMENT environment_parent(ENVIRONMENT environment);
void environment_add(ENVIRONMENT environment,void *key,void *value);
void *environment_search(ENVIRONMENT environment,void *key);
void environment_print(ENVIRONMENT environment);

#endif

