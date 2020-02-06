#ifndef __PARSE_H_INCLUDED__
#define __PARSE_H_INCLUDED__
#include <cell.h>

struct parser {
    Cell *cell;
    char **next;
};

typedef struct parser Parser;

Parser *parse(char **tokens);
#endif
