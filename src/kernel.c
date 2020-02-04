#include<stdlib.h>
#include<stdio.h>
#include<kernel.h>
Cell *adder(ENVIRONMENT *bst, Cell *list){
    long l = 0;
    while(list){
        l = l + (long)list->car;
        list = list->cdr;
    }
    Cell *c = cons((void *)l,0,TYPE_INT);
    return c;
}
Cell *def(ENVIRONMENT envir, Cell *list){
    Cell *c;
    if(list->type == TYPE_SYMBOL){
        environment_add(envir,list->car,list->cdr);
        c = cons(0,0,TYPE_TRUE);
    } else{
        c = cons(0,0,TYPE_NIL);
    }
    return c;
}
Cell *is_atom(ENVIRONMENT environment, Cell *list){
    if(cons_len(list) != 1){
      Cell *c = cons(0,0,TYPE_NIL);
      return c;
    }
    Cell *c = cons(0,0,TYPE_TRUE);
    if(c->type == TYPE_LIST) c->type = TYPE_NIL;
    return c;
}
