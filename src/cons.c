#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cons.h>
#include <bst.h>

Cell *cons(void *car,void *cdr,short type){
    Cell *p;
    p = (Cell *)malloc(sizeof(Cell));
    p->car = car;
    p->cdr = cdr;
    p->type = type;
    return p;
}
void *car(Cell *cons){
    return cons->car;
}
void *cdr(Cell *cons){
    return cons->cdr;
}
int cons_len(Cell *cons){
    int i=0;
    while(cons){
        cons = cons->cdr;
        i++;
    }
    return i;
}
void cons_print_(Cell *cons){
    if (cons->type == TYPE_SYMBOL){
      printf(" %s ",(char*)cons->car);
    }
    if (cons->type == TYPE_INT){
      printf(" %ld ",(long)cons->car);
    }
    if (cons->type == TYPE_NIL){
      printf(" nil ");
    }
    if (cons->type == TYPE_TRUE){
      printf(" #t ");
    }
    if (cons->type == TYPE_NATIVE){
      printf(" @native@ ");
    }
    if (cons->type == TYPE_LAMBDA){
        printf("{");
        cons_print_(cons->car);
        cons_print_(((Cell *)(cons->car))->cdr);
        printf("}");
    }
    if(cons->type == TYPE_LIST){
        printf("(");
        Cell *cc = cons->car;
        while(cc){
          cons_print_(cc);
          cc = cc->cdr;
        }
        printf(")");
    }   
}
