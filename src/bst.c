#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bst.h>


Node *root = 0;
BST *bst_new(BST_CMP_FN fn){
    BST *bst = malloc(sizeof(BST));
    bst->root = 0;
    bst->compare_fn = fn;
    return bst;
}

Node *bst_insert1(BST *bst,Node *tree, void *key,void *value){
    if(tree == 0){
        Node *n = malloc(sizeof(Node));
        n->parent = 0;
        n->left = 0;
        n->right=0;
        n->key = key;
        n->value = value;
        if (bst->root == 0){
            bst->root = n;
        }
        return n;
    }else if (bst->compare_fn(key,tree->key) < 0){
        tree->left = bst_insert1(bst,tree->left,key,value);
        tree->left->parent = tree;
    }else if (bst->compare_fn(key,tree->key) > 0){
        tree->right = bst_insert1(bst,tree->right,key,value);
        tree->right->parent = tree;
    }
    return tree;
}
Node *bst_insert(BST *bst,void *key,void *value){
    return bst_insert1(bst,bst->root,key,value);
}
void *bst_search1(BST *bst,Node *tree,void *key){
    if (tree == 0){
        return 0;
    }
    if (!bst->compare_fn(tree->key,key)){
        return tree->value;
    }
    if(bst->compare_fn(key,tree->key) > 0){
        return bst_search1(bst,tree->right,key);
    }else{
        return bst_search1(bst,tree->left,key);
    }
}
void *bst_search(BST *bst,void *key){
    return bst_search1(bst,bst->root,key);
}
