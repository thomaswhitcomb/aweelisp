#include <stdlib.h>
#include <stdio.h>
#include <cons.h>
#include <eval.h>
#include <bst.h>

Cell *eval(BST *envir,Cell *c){
    if (c == 0){
        return c;
    }
    Cell *curr, *prev = 0, *root = 0;
    while(c){
        if(c->type == TYPE_SYMBOL){
            curr = bst_search(envir,c->car);
            if(curr == 0){
                curr = cons(c->car,0,TYPE_SYMBOL);
            }else{
                curr = cons(curr,0,TYPE_NATIVE);
            }
        }
        if(c->type == TYPE_INT){
            curr = cons(c->car,0,TYPE_INT);
        }
        if(c->type == TYPE_LAMBDA){
            if(cons_len(c->car) != 2 || (((Cell *)(c->car))->type != TYPE_LIST && c->cdr->cdr->type != TYPE_LIST)){
               puts("bad lambda definition");
               Cell *c = cons(0,0,TYPE_NIL);
               return c;
            }   
            curr = c;
        }
        if(c->type == TYPE_LIST){
            Cell *list = (Cell *)c->car;
            Cell *ret = eval(envir,c->car);
            Cell *parms = eval(envir,list->cdr);
            if(ret->type == TYPE_NATIVE){
                curr = ((LAMBDA)ret->car)(envir,parms);
            }else{
                if (ret->type == TYPE_LAMBDA){
                    printf("executing: ");
                    cons_print(ret);
                    curr = eval(envir,((Cell *)ret->car)->cdr);
                }else{
                    printf("yikes - invalid functor: %i\n",ret->type);
                }
            }
        }
        if(root == 0) root = curr;
        if(prev != 0) prev->cdr = curr;
        prev = curr;

        c = c->cdr;
    }
    return root;
}
