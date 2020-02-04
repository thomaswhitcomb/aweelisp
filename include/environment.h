#ifndef __ENVIRONMENT_H_INCLUDED__   
#define __ENVIRONMENT_H_INCLUDED__
#include <bst.h>

struct environment{
    struct environment *parent;
    struct environment *child;
    BST *bst;
};
typedef struct environment ENVIRONMENT;

ENVIRONMENT *environment_new(ENVIRONMENT *parent);
ENVIRONMENT *environment_parent(ENVIRONMENT *environment);
ENVIRONMENT *environment_child(ENVIRONMENT *environment);
void environment_add(ENVIRONMENT *environment,void *key,void *value);
void *environment_search(ENVIRONMENT *environment,void *key);
void environment_print(ENVIRONMENT *environment);

#endif

