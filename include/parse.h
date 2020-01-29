#ifndef __PARSE_H_INCLUDED__
#define __PARSE_H_INCLUDED__
#include <cons.h>

struct parser {
    Cell *cons;
    char **next;
};

typedef struct parser Parser;

Parser *parse(char **tokens);
#endif
