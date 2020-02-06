#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell.h>
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
            curr = cell_new(pp->cell,0,TYPE_LAMBDA);
          }else{
            pp = parse(tokens+1);
            curr = cell_new(pp->cell,0,TYPE_LIST);
          }
          tokens = pp->next;
          
          if (cs == 0) cs = curr;
          if(prev != 0) prev->next = curr;
          prev = curr;

      } else{
          if(!strcmp(")",*tokens)){
              Parser *pp = (Parser*)malloc(sizeof(Parser));
              pp->cell =cs;
              pp->next = tokens;
              return pp;
          } else{
              char *result;
              long l = strtol(*tokens,&result,10);
              Cell *curr;
              if (*result == 0){
                  curr = cell_new((void *) l,0,TYPE_INT);
              }else{
                  curr = cell_new(*tokens,0,TYPE_SYMBOL);
              }
              if (cs == 0) cs = curr;
              if(prev != 0) prev->next = curr;
              prev = curr;
          }
      }
      tokens++;
  }
  Parser *pp = (Parser*)malloc(sizeof(Parser));
  pp->cell =cs;
  pp->next = 0;
  return pp;
}
