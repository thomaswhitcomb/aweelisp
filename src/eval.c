#include <stdlib.h>
#include <stdio.h>
#include <cons.h>
#include <eval.h>
#include <bst.h>
#include <environment.h>
#include <tokenize.h>
#include <parse.h>
#include <native.h>
#include <string.h>
Cell *eval_main(char *str){
    int parens = 0;
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *prev,*cons = p->cons;
    Cell *result = 0;
    while(cons){
        Cell *c = eval(environment,cons);
        cons = cons->cdr;
        if(result == 0){
            result = c;
        }else{
            prev->cdr = c;
        }
        prev = c;

    }
    return result;
}
Cell *eval_params(ENVIRONMENT *envir,Cell *p){
    Cell *actual_params = p;
    Cell *pv;
    Cell *prev = 0;
    while(p){
      pv = eval(envir,p);
      if(prev != 0) prev->cdr = pv;
      else{
          actual_params = pv;
      }
      prev = pv;
      p = p->cdr;
    }
    return actual_params;
}
Cell *eval(ENVIRONMENT *envir,Cell *c){
    if (c == 0){
        return c;
    }
    //printf("In eval: ");
    //cons_print(c);
    Cell *curr, *prev = 0, *root = 0;
    if(c->type == TYPE_SYMBOL){
        //printf("symbol lookup for: %s\n",c->car);
        //environment_print(envir);
        curr = native_fetch(c->car);
        if (!curr){
            curr = environment_search(envir,c->car);
            if(!curr){
                curr = cons(c->car,0,TYPE_SYMBOL);
            }
        }
        return curr;
    }
    if(c->type == TYPE_INT){
        curr = cons(c->car,0,TYPE_INT);
        return curr;
    }
    if(c->type == TYPE_LAMBDA){
        if(cons_len(c->car) != 2 || (((Cell *)(c->car))->type != TYPE_LIST && c->cdr->cdr->type != TYPE_LIST)){
           puts("bad lambda definition");
           return cons(0,0,TYPE_NIL);
        }   
        return c;
    }
    if(c->type == TYPE_LIST){
        Cell *functor = eval(envir,c->car);
        Cell *p = ((Cell*)(c->car))->cdr;
        Cell *actual_params = p;
        if(functor->type == TYPE_NATIVE){
            if(native_eval_params(functor->car)){
                actual_params = eval_params(envir,p);
            }
            curr = ((LAMBDA)native_ptr(functor->car))(envir,actual_params);
        }else{
            if (functor->type == TYPE_LAMBDA){
                //printf("executing: ");
                //cons_print(functor);
                Cell *formal_params = functor->car; //PARAMETER LIST
                Cell *formal_param_name = formal_params->car;
                Cell *code = ((Cell *)functor->car)->cdr;
                ENVIRONMENT *child = environment_child(envir);
                if(!child){
                    child = environment_new(envir);
                }
                actual_params = eval_params(envir,p);
                while(formal_param_name){
                    Cell *p = cons(actual_params->car,0,actual_params->type);
                    environment_add(child,formal_param_name->car,p);
                    actual_params = actual_params->cdr;
                    formal_param_name = formal_param_name->cdr;
                }
                curr = eval(child,code);
            }else{
                printf("yikes - invalid functor: %i\n",functor->type);
                curr = cons(0,0,TYPE_NIL);
            }
        }
        return curr;
    }
    puts("should not have gotten here");
    return cons(0,0,TYPE_NIL);
}
