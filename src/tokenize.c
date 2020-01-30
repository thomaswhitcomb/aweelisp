#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokenize.h>
char * prepro(char *str){
    int inliteral = 0;
    char *pad = malloc(2*strlen(str));
    char *start = pad;
    *start = 0;

    while(*str) {
        if (*str == '"' && inliteral == 1) inliteral = 0; 
        else if (*str == '"' && inliteral == 0) inliteral = 1;
        else {
            if (inliteral == 0){
                if(*str == '(' || *str == ')') {
                    if (pad == start ){
                        *pad++ = *str;
                        *pad++ = ' ';
                    }else{
                        if(*(pad -1) != ' '){
                            *pad++ = ' ';
                        }
                        *pad++ = *str;
                        *pad++ = ' ';
                    }
                } else{
                    if(*str == ' '){
                        if (pad != start && *(pad-1) != ' ') *pad++ = ' ';
                    } else *pad++ = *str;
                }
            }else{
                *pad++ = *str;
            }
        }
        str++;
    }
    *pad = 0;
    return start;
}
char** tokenize(char *str,int *parens){
    char *padded = prepro(str);
    char *token = strtok(padded, " ");
    int count = 0;
    while(token) {
      count++;
      token = strtok(NULL, " ");
    }
    *parens = 0;
    char **array = malloc(sizeof(char*)*(count+1));
    char **a = array;
    char *p = padded;
    while (*p){
        if(*p == '(') (*parens)--;
        if(*p == ')') (*parens)++;
        *a++ = p;
        p = p + strlen(p) + 1;
    }
    *a = 0;
    return array;
}
