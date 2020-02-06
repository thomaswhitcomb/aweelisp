#include <stdlib.h>
#include <stdio.h>
#include <cell.h>
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
    Cell *prev,*cell = p->cell;
    Cell *result = 0;
    while(cell){
        Cell *c = eval(environment,cell);
        cell = cell->next;
        if(result == 0){
            result = c;
        }else{
            prev->next = c;
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
      if(prev != 0) prev->next = pv;
      else{
          actual_params = pv;
      }
      prev = pv;
      p = p->next;
    }
    return actual_params;
}
Cell *eval(ENVIRONMENT *envir,Cell *c){
    if (c == 0){
        return c;
    }
    //printf("In eval: ");
    //cell_print(c);
    Cell *curr, *prev = 0, *root = 0;
    if(c->type == TYPE_SYMBOL){
        //printf("symbol lookup for: %s\n",c->datum);
        //environment_print(envir);
        curr = environment_search(envir,c->datum);
        if (!curr){
            curr = native_fetch(c->datum);
            if(!curr){
                curr = cell_new(c->datum,0,TYPE_SYMBOL);
            }
        }
        return curr;
    }
    if(c->type == TYPE_INT){
        curr = cell_new(c->datum,0,TYPE_INT);
        return curr;
    }
    if(c->type == TYPE_LAMBDA){
        if(cell_len(c->datum) != 2 || (((Cell *)(c->datum))->type != TYPE_LIST && c->next->next->type != TYPE_LIST)){
           puts("bad lambda definition");
           return cell_new(0,0,TYPE_NIL);
        }   
        return c;
    }
    if(c->type == TYPE_LIST){
        Cell *functor = eval(envir,c->datum);
        Cell *p = ((Cell*)(c->datum))->next;
        Cell *actual_params = p;
        if(functor->type == TYPE_NATIVE){
            if(native_eval_params(functor->datum)){
                actual_params = eval_params(envir,p);
            }
            curr = ((LAMBDA)native_ptr(functor->datum))(envir,actual_params);
        }else{
            if (functor->type == TYPE_LAMBDA){
                //printf("executing: ");
                //cell_print(functor);
                Cell *formal_params = functor->datum; //PARAMETER LIST
                Cell *formal_param_name = formal_params->datum;
                Cell *code = ((Cell *)functor->datum)->next;
                ENVIRONMENT *child = environment_child(envir);
                if(!child){
                    child = environment_new(envir);
                }
                actual_params = eval_params(envir,p);
                while(formal_param_name){
                    Cell *p = cell_new(actual_params->datum,0,actual_params->type);
                    environment_add(child,formal_param_name->datum,p);
                    actual_params = actual_params->next;
                    formal_param_name = formal_param_name->next;
                }
                curr = eval(child,code);
            }else{
                printf("yikes - invalid functor: %i\n",functor->type);
                curr = cell_new(0,0,TYPE_NIL);
            }
        }
        return curr;
    }
    puts("should not have gotten here");
    return cell_new(0,0,TYPE_NIL);
}
