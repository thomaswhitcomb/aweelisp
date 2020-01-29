#include <stdlib.h>
#ifndef __BST_H_INCLUDED__   
#define __BST_H_INCLUDED__

struct node {
    void *key;
    void *value;
    struct node *parent;
    struct node *left;
    struct node *right;
};

typedef struct node Node;

typedef int (*BST_CMP_FN)(void *,void *);
struct bst {
    Node *root;
    BST_CMP_FN compare_fn;
};
typedef struct bst BST;

BST *bst_new(BST_CMP_FN);
void *bst_search(BST *bst,void *key);
Node *bst_insert(BST *bst,void *key, void* value);

#endif

