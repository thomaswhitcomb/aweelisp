#include<stdlib.h>
#include<stdio.h>
#include<native.h>
#include<bst.h>
#include<cell.h>
#include<string.h>
#include<eval.h>

BST *dictionary = 0;
static int mystrcmp (void *p1,void*p2){
    return strcmp((char *)p1,(char *)p2);
}

static Cell *is_atom(ENVIRONMENT *environment, Cell *list);
static Cell *def(ENVIRONMENT *envir, Cell *list);
static Cell *adder(ENVIRONMENT *envir, Cell *list);
static Cell *quote(ENVIRONMENT *envir, Cell *list);
static Cell *cons(ENVIRONMENT *envir, Cell *list);
static Cell *car(ENVIRONMENT *envir, Cell *list);
static Cell *cdr(ENVIRONMENT *envir, Cell *list);
static Cell *list(ENVIRONMENT *envir, Cell *list);
static Cell *eq(ENVIRONMENT *envir, Cell *list);

static Cell *native_alloc(void *func,int eval_params){
    return cell_new((void *)((unsigned long)func | eval_params),0,TYPE_NATIVE);
}

void native_initialize(){
    dictionary = bst_new(mystrcmp);
    bst_insert(dictionary,"+",native_alloc(adder,1));
    bst_insert(dictionary,"def",native_alloc(def,0));
    bst_insert(dictionary,"atom",native_alloc(is_atom,1));
    bst_insert(dictionary,"quote",native_alloc(quote,0));
    bst_insert(dictionary,"cons",native_alloc(cons,1));
    bst_insert(dictionary,"car",native_alloc(car,1));
    bst_insert(dictionary,"cdr",native_alloc(cdr,1));
    bst_insert(dictionary,"list",native_alloc(list,1));
    bst_insert(dictionary,"eq",native_alloc(eq,1));
}
int native_eval_params(LAMBDA lambda){
    return ((unsigned long)lambda & 1);
}

LAMBDA native_ptr(LAMBDA lambda){
    return (LAMBDA) ((unsigned long)lambda & ~(1));
}
Cell *native_fetch(char *name){
    Cell *cell = (Cell *)bst_search(dictionary,name);
    return cell;
}

static Cell *adder(ENVIRONMENT *envir, Cell *list){
    long l = 0;
    while(list){
        l = l + (long)list->datum;
        list = list->next;
    }
    Cell *c = cell_new((void *)l,0,TYPE_INT);
    return c;
}
static Cell *def(ENVIRONMENT *envir, Cell *list){
    Cell *c;
    if(list->type == TYPE_SYMBOL){
        environment_add(envir,list->datum,list->next);
        c = cell_new(0,0,TYPE_TRUE);
    } else{
        c = cell_new(0,0,TYPE_LIST);
    }
    return c;
}
static Cell *is_atom(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) != 1){
        Cell *c = cell_new(0,0,TYPE_LIST);
        return c;
    }
    Cell *c = cell_new(0,0,TYPE_TRUE);
    if(list->type == TYPE_LIST) c->type = TYPE_LIST;
    if(list->type == TYPE_LAMBDA) c->type = TYPE_LIST;
    return c;
}
static Cell *quote(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) == 0){
        return cell_new(0,0,TYPE_LIST);
    }
    return list;
}
static Cell *cons(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) != 2){
        return cell_new(0,0,TYPE_LIST);
    }
    if(list->next->type != TYPE_LIST){
        return cell_new(0,0,TYPE_LIST);
    }
    Cell *clone = cell_clone(list);
    Cell *clone_next = clone->next;
    clone->next = clone_next->datum;
    return cell_new(clone,0,TYPE_LIST);
}
static Cell *car(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) != 1){
        return cell_new(0,0,TYPE_LIST);
    }
    if(list->type != TYPE_LIST){
        return cell_new(0,0,TYPE_LIST);
    }
    if(list->datum == 0){
        return cell_new(0,0,TYPE_NIL);
    }
    Cell *first = (Cell *)list->datum;
    Cell *save = first->next;
    first->next = 0;
    Cell *clone = cell_clone(first);
    first->next = save;
    return clone;
}
static Cell *cdr(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) != 1){
        return cell_new(0,0,TYPE_LIST);
    }
    if(list->type != TYPE_LIST){
        return cell_new(0,0,TYPE_LIST);
    }
    // zero lenth list
    if(list->datum == 0){
        return cell_new(0,0,TYPE_LIST);
    }
    // one element list
    if(((Cell *)list->datum)->next == 0){
        return cell_new(0,0,TYPE_LIST);
    }
    Cell *clone = cell_clone(((Cell *)list->datum)->next);
    Cell *l = cell_new(clone,0,TYPE_LIST);
    return l;
}
static Cell *list(ENVIRONMENT *environment, Cell *list){
    Cell *clone = cell_clone(list);
    Cell *ret = cell_new(clone,0,TYPE_LIST);
    return ret;
}
static Cell *eq(ENVIRONMENT *environment, Cell *list){
    if(cell_len(list) != 2){
        return cell_new(0,0,TYPE_LIST);
    }
    Cell * ret = 0;
    if (list->type != list->next->type){
        return cell_new(0,0,TYPE_LIST);
    }
    if(list->type == TYPE_SYMBOL){
        if(!strcmp(list->datum,list->next->datum)){
            return cell_new(0,0,TYPE_TRUE);
        }else{
            return cell_new(0,0,TYPE_LIST);
        }
    }else {
        if(list->type == TYPE_INT){
            if((long)list->datum == (long)list->next->datum){
                return cell_new(0,0,TYPE_TRUE);
            }else{
                return cell_new(0,0,TYPE_LIST);
            }
        }else {
            if(list->type == TYPE_LIST && 
               list->datum == 0 && 
               list->next->datum == 0) 
                return cell_new(0,0,TYPE_TRUE);
            else 
                return cell_new(0,0,TYPE_LIST);
        }
    }
}
