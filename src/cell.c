#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell.h>
#include <bst.h>

Cell *cell_new(void *datum,void *next,short type){
    Cell *p;
    p = (Cell *)malloc(sizeof(Cell));
    p->datum = datum;
    p->next = next;
    p->type = type;
    return p;
}
int cell_len(Cell *cell){
    int i=0;
    while(cell){
        cell = cell->next;
        i++;
    }
    return i;
}
void cell_print_(Cell *cell){
    if (cell->type == TYPE_SYMBOL){
      printf(" %s ",(char*)cell->datum);
    }
    if (cell->type == TYPE_INT){
      printf(" %ld ",(long)cell->datum);
    }
    if (cell->type == TYPE_NIL){
      printf(" nil ");
    }
    if (cell->type == TYPE_TRUE){
      printf(" #t ");
    }
    if (cell->type == TYPE_NATIVE){
      printf(" @native@ ");
    }
    if (cell->type == TYPE_LAMBDA){
        printf("{");
        cell_print_(cell->datum);
        cell_print_(((Cell *)(cell->datum))->next);
        printf("}");
    }
    if(cell->type == TYPE_LIST){
        printf("(");
        Cell *cc = cell->datum;
        while(cc){
          cell_print_(cc);
          cc = cc->next;
        }
        printf(")");
    }   
}
Cell *cell_clone(Cell *cell){
    if(cell == 0){
        return 0;
    }
    Cell *right,*left,*new;
    if (cell->type == TYPE_LAMBDA || cell->type == TYPE_LIST){
        left = cell_clone((Cell *)cell->datum);
        right = cell_clone(cell->next);
        new = cell_new(left,right,cell->type);
    } else {
        right = cell_clone(cell->next);
        new = cell_new(cell->datum,right,cell->type);
    }   
    return new;
}
int cell_equal(Cell *cell1,Cell *cell2){
    if(cell1 == 0 && cell2 == 0){
        return 1;
    }
    int right,left;
    if (cell1->type == cell2->type){
        if (cell1->type == TYPE_LAMBDA || cell1->type == TYPE_LIST){
           left = cell_equal(cell1->datum,cell2->datum);
           right = cell_equal(cell1->next,cell2->next);
           return left && right;
        }else{
            right = cell_equal(cell1->next,cell2->next);
            return right;
        }
    }else{
        return 0;
    }

}
