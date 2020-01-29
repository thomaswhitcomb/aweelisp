#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cons.h>
#include <parse.h>

Parser* parse(char **tokens){
  Cell *prev = 0;
  Cell *cs = 0;
  while(*tokens){
      if(!strcmp("(",*tokens)){
          Cell *curr = 0;
          Parser *pp = 0;
          if(!strcmp(*(tokens+1),"lambda")){
            pp = parse(tokens+2);
            curr = cons(pp->cons,0,TYPE_LAMBDA);
          }else{
            pp = parse(tokens+1);
            curr = cons(pp->cons,0,TYPE_LIST);
          }
          tokens = pp->next;
          
          if (cs == 0) cs = curr;
          if(prev != 0) prev->cdr = curr;
          prev = curr;

      } else{
          if(!strcmp(")",*tokens)){
              Parser *pp = (Parser*)malloc(sizeof(Parser));
              pp->cons =cs;
              pp->next = tokens;
              return pp;
          } else{
              char *result;
              long l = strtol(*tokens,&result,10);
              Cell *curr;
              if (*result == 0){
                  curr = cons((void *) l,0,TYPE_INT);
              }else{
                  curr = cons(*tokens,0,TYPE_SYMBOL);
              }
              if (cs == 0) cs = curr;
              if(prev != 0) prev->cdr = curr;
              prev = curr;
          }
      }
      tokens++;
  }
  Parser *pp = (Parser*)malloc(sizeof(Parser));
  pp->cons =cs;
  pp->next = 0;
  return pp;
}
