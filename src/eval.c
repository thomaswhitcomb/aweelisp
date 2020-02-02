#include <stdlib.h>
#include <stdio.h>
#include <cons.h>
#include <eval.h>
#include <bst.h>
#include <environment.h>

Cell *eval(ENVIRONMENT envir,Cell *c){
    if (c == 0){
        return c;
    }
    Cell *curr, *prev = 0, *root = 0;
    while(c){
        if(c->type == TYPE_SYMBOL){
            curr = environment_search(envir,c->car);
            if(curr == 0){
                curr = cons(c->car,0,TYPE_SYMBOL);
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
            Cell *actual_params = eval(envir,list->cdr);
            if(ret->type == TYPE_NATIVE){
                curr = ((LAMBDA)ret->car)(envir,actual_params);
            }else{
                if (ret->type == TYPE_LAMBDA){
                    printf("executing: ");
                    cons_print(ret);
                    Cell *formal_params = ret->car; //PARAMETER LIST
                    Cell *code = ((Cell *)ret->car)->cdr;
                    Cell *formal_param_name = formal_params->car;
                    ENVIRONMENT envir1 = environment_new(envir);
                    while(formal_param_name){
                        printf("param is: %s\n",formal_param_name->car);
                        environment_add(envir1,formal_param_name->car,actual_params);
                        actual_params = actual_params->cdr;
                        formal_param_name = formal_param_name->cdr;
                    }
                    curr = eval(envir1,code);
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
