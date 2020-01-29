#ifndef __CELL_H_INCLUDED__   
#define __CELL_H_INCLUDED__

#define TYPE_STRING  0
#define TYPE_SYMBOL  1
#define TYPE_LIST    2
#define TYPE_INT     3
#define TYPE_NIL     4
#define TYPE_TRUE    5
#define TYPE_LAMBDA  6
#define TYPE_NATIVE  7

#define cons_print(cons) do {cons_print_(cons);printf("\n");} while(0)

struct cell {
    void *car;
    struct cell *cdr;
    short type;
};

typedef struct cell Cell;

//P R O T O T Y P E S
Cell *cons(void *car, void *cdr,short type);
void *car(Cell *cons);
void *cdr(Cell *cons);
void cons_print_(Cell *cons);
int cons_len(Cell *cons);
#endif
