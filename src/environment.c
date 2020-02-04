#include <stdlib.h>
#include <stdio.h>
#include <environment.h>
#include <bst.h>
#include <string.h>
static int mystrcmp (void *p1,void*p2){
      return strcmp((char *)p1,(char *)p2);
}
ENVIRONMENT environment_new(ENVIRONMENT parent){
    ENVIRONMENT e = malloc(sizeof(ENVIRONMENT));
    e->parent = parent;
    e->bst = bst_new(mystrcmp);
    return e;
}
void environment_add(ENVIRONMENT envir,void *key,void *value){
    bst_insert(envir->bst,key,value);
}
void environment_print(ENVIRONMENT envir){
    int i=0;
    while(envir){
      printf("environment print level %i\n",i);
      bst_print(envir->bst);
      envir = envir->parent;
      i++;
    }
}
void* environment_search(ENVIRONMENT envir,void *key){
    void *value = 0;
    while(envir && !value){
        value = bst_search(envir->bst,key);
        envir = envir->parent;
    }
    return value;
}

