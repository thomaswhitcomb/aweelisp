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

#define cell_print(cons) do {cell_print_(cons);printf("\n");} while(0)

struct cell {
    struct cell *next;
    void *datum;
    short type;
};

typedef struct cell Cell;

//P R O T O T Y P E S
Cell *cell_new(void *datum, void *next,short type);
Cell *cell_clone(Cell *cell);
int cell_equal(Cell *cell1, Cell *cell2);
void cell_print_(Cell *cell);
int cell_len(Cell *cell);
#endif
