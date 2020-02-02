#include <minunit.h>
#include <stdio.h>
#include <string.h>
#include <parse.h>
#include <tokenize.h>
#include <bst.h>
#include <eval.h>
#include <environment.h>

int tests_run = 0;
int tests_error = 0;
 
static char *test_missing_rparen(){
    char str[] = "Da ( ()";
    int parens;
    char **array = tokenize(str,&parens);
    mu_assert("Error - test_missing_rparen 1.0",parens == -1);
    return 0;
}
static char *test_missing_lparen(){
    char str[] = "Da (dsd (d) ab)) a)d";
    int parens;
    char **array = tokenize(str,&parens);
    mu_assert("Error - test_missing_lparen 1.0",parens == 2);
    return 0;
}
static char *test_prepro1(){
    char str[] = "hello";
    char *pad = prepro(str);
    mu_assert("Error - prepro test 1.1",!strcmp("hello",pad));
    mu_assert("Error - prepro test 1.2",strlen(pad) == 5);
    return 0;
}
static char *test_prepro2(){
    char str[] = "he(lo";
    char *pad = prepro(str);
    //printf("%s\n",pad);
    mu_assert("Error - prepro test 2",!strcmp("he ( lo",pad));
    return 0;
}
static char *test_prepro3(){
    char str[] = "he      (lo";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 3",!strcmp("he ( lo",pad));
    return 0;
}
static char *test_prepro4(){
    char str[] = "he    (   lo";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 4",!strcmp("he ( lo",pad));
    return 0;
}
static char *test_prepro5(){
    char str[] = "           ";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 5",!strcmp("",pad));
    return 0;
}
static char *test_prepro6(){
    char str[] = " ";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 6",!strcmp("",pad));
    return 0;
}
static char *test_prepro7(){
    char str[] = "";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 7",!strcmp("",pad));
    return 0;
}
static char *test_prepro8(){
    char str[] = "he    ()   lo";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 8",!strcmp("he ( ) lo",pad));
    return 0;
}
static char *test_prepro9(){
    char str[] = "(";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 9",!strcmp("( ",pad));
    return 0;
}
static char *test_prepro10(){
    char str[] = ")";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 10",!strcmp(") ",pad));
    return 0;
}
static char *test_prepro11(){
    char str[] = ") ";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 11",!strcmp(") ",pad));
    return 0;
}
static char *test_prepro12(){
    char str[] = "(a b c d)";
    char *pad = prepro(str);
    mu_assert("Error - prepro test 12",!strcmp("( a b c d ) ",pad));
    return 0;
}
static char *test_prepro13(){
    char str[] = "(a b (c 2) ( blad 12121 abc ) d)";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 13",!strcmp("( a b ( c 2 ) ( blad 12121 abc ) d ) ",pad));
    return 0;
}
static char *test_prepro14(){
    char str[] = "(()())";
    char *pad = prepro(str);
    //printf("%s -> %s\n",str,pad);
    mu_assert("Error - prepro test 14",!strcmp("( ( ) ( ) ) ",pad));
    return 0;
}
static char *test_prepro15(){
    char str[] = "((((((";
    char *pad = prepro(str);
    mu_assert("Error - prepro test 15",!strcmp("( ( ( ( ( ( ",pad));
    return 0;
}
static char *test_tokenize1(){
    int parens;
    char str[] = "(()())";
    char **a = tokenize(str,&parens);
    mu_assert("Error - tokenize1.0",!strcmp(*a,"("));
    a++;
    mu_assert("Error - tokenize1.1",!strcmp(*a,"("));
    a++;
    mu_assert("Error - tokenize1.2",!strcmp(*a,")"));
    a++;
    mu_assert("Error - tokenize1.3",!strcmp(*a,"("));
    a++;
    mu_assert("Error - tokenize1.4",!strcmp(*a,")"));
    a++;
    mu_assert("Error - tokenize1.5",!strcmp(*a,")"));
    a++;
    mu_assert("Error - tokenize1.6",*a == 0);

    return 0;
}
static char *test_tokenize2(){
    int parens;
    char str[] = "hi";
    char **array = tokenize(str,&parens);
    mu_assert("Error - test_tokenize2.1",!strcmp(array[0],"hi"));
    mu_assert("Error - test_tokenize2.2",array[1] == 0);
    return 0;
}
static char *test_tokenize3(){
    int parens;
    char str[] = "  (* 2 (+ x 7)) ";
    char **a = tokenize(str,&parens);
    mu_assert("Error - tokenize3.0",!strcmp(*a,"("));
    a++;
    mu_assert("Error - tokenize3.1",!strcmp(*a,"*"));
    a++;
    mu_assert("Error - tokenize3.2",!strcmp(*a,"2"));
    a++;
    mu_assert("Error - tokenize3.3",!strcmp(*a,"("));
    a++;
    mu_assert("Error - tokenize3.4",!strcmp(*a,"+"));
    a++;
    mu_assert("Error - tokenize3.5",!strcmp(*a,"x"));
    a++;
    mu_assert("Error - tokenize3.6",!strcmp(*a,"7"));
    a++;
    mu_assert("Error - tokenize3.7",!strcmp(*a,")"));
    a++;
    mu_assert("Error - tokenize3.8",!strcmp(*a,")"));
    a++;
    mu_assert("Error - tokenize3.9",*a == 0);

    return 0;
}
static char *test_parse1(){
    int parens;
    char str[] = "hi bye";
    char **array = tokenize(str,&parens);
    mu_assert("Error - test_parse1.1",!strcmp(*array,"hi"));
    mu_assert("Error - test_parse1.2",!strcmp(*(array+1),"bye"));
    Parser *p = parse(array);
    return 0;
}
static char *test_parse2(){
    int parens;
    char str[] = "(hi bye)";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    mu_assert("Error - test_parse2.1",p->cons->type == TYPE_LIST);
    Cell *list = p->cons->car;
    mu_assert("Error - test_parse2.2",!strcmp(list->car,"hi"));
    mu_assert("Error - test_parse2.3",!strcmp(list->cdr->car,"bye"));
    return 0;
}
static char *test_parse3(){
    char str[] = "(spic (+ 2 3) span)";
    int parens;
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    mu_assert("Error - test_parse3.1",p->cons->type == TYPE_LIST);
    Cell *list = p->cons->car;
    mu_assert("Error - test_parse3.2",!strcmp(list->car,"spic"));
    mu_assert("Error - test_parse3.3",list->cdr->type == TYPE_LIST);
    mu_assert("Error - test_parse3.4",list->cdr->cdr->type == TYPE_SYMBOL);
    mu_assert("Error - test_parse3.5",!strcmp(list->cdr->cdr->car,"span"));
    list = (Cell *)list->cdr->car;
    mu_assert("Error - test_parse3.6",!strcmp(list->car,"+"));
    mu_assert("Error - test_parse3.7",(long) list->cdr->car == 2);
    mu_assert("Error - test_parse3.8",(long) list->cdr->cdr->car == 3);
    mu_assert("Error - test_parse3.9",list->cdr->cdr->cdr == 0);
    return 0;
}
static int myintcmp (void *p1,void*p2){
    if((int) p1 == (int)p2) return 0;
    else if((int) p1 > (int)p2) return -1;
    else return 1;
}
static int mystrcmp (void *p1,void*p2){
    return strcmp((char *)p1,(char *)p2);
}
static char *test_bst1(){
    BST *bst = bst_new(myintcmp);
    bst_insert(bst,(void *)25,(void *)25);
    bst_insert(bst,(void *)15,(void *)15);
    bst_insert(bst,(void *)50,(void *)50);
    bst_insert(bst,(void *)10,(void *)10);
    bst_insert(bst,(void *)22,(void *)22);
    bst_insert(bst,(void *)35,(void *)35);
    bst_insert(bst,(void *)70,(void *)70);
    bst_insert(bst,(void *)4,(void *)4);
    bst_insert(bst,(void *)12,(void *)12);
    bst_insert(bst,(void *)18,(void *)18);
    bst_insert(bst,(void *)24,(void *)24);
    bst_insert(bst,(void *)31,(void *)31);
    bst_insert(bst,(void *)44,(void *)44);
    bst_insert(bst,(void *)66,(void *)66);
    bst_insert(bst,(void *)90,(void *)90);

    void *result = bst_search(bst,(void *)31);
    mu_assert("Error - test_bst1 ",result == (void *)31);
    result = bst_search(bst,(void *)25);
    mu_assert("Error - test_bst2 ",result == (void *)25);
    result = bst_search(bst,(void *)18);
    mu_assert("Error - test_bst3 ",result == (void *)18);
    result = bst_search(bst,(void *)1000);
    mu_assert("Error - test_bst2 ",result == 0);
    return 0;
}


Cell *adder(ENVIRONMENT *bst, Cell *list){
    long l = 0;
    while(list){
        l = l + (long)list->car;
        list = list->cdr;
    }
    Cell *c = cons((void *)l,0,TYPE_INT);
    return c;
}
Cell *def(ENVIRONMENT envir, Cell *list){
    Cell *c;
    if(list->type == TYPE_SYMBOL){
        environment_add(envir,list->car,list->cdr);
        c = cons(0,0,TYPE_TRUE);
    } else{
        c = cons(0,0,TYPE_NIL);
    }
    return c;
}
Cell *is_atom(ENVIRONMENT environment, Cell *list){
    if(cons_len(list) != 1){
      Cell *c = cons(0,0,TYPE_NIL);
      return c;
    }
    Cell *c = cons(0,0,TYPE_TRUE);
    if(c->type == TYPE_LIST) c->type = TYPE_NIL;
    return c;
}

static char *test_eval1(){
    //char str[] = "(+ 2 (+ 5 7 3))";
    char str[] = "(+ (+ 2 (+ 5 7 3)) 10)";
    int parens;
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - eval1.1 ",c != 0);
    mu_assert("Error - eval1.2 ",c -> cdr == 0);
    mu_assert("Error - eval1.3 ",(long) c -> car == 27);
    return 0;
}
static char *test_eval2(){
    int parens;
    char str[] = "(atom 5)";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - eval2.1 ",c != 0);
    mu_assert("Error - eval2.2 ",c -> cdr == 0);
    mu_assert("Error - eval2.3 ",c -> car == 0);
    mu_assert("Error - eval2.4 ",c -> type == TYPE_TRUE);
    return 0;
}
static char *test_eval3(){
    int parens;
    char str[] = "abc (+ 4 5) xyz)";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - eval3.1 ",c != 0);
    mu_assert("Error - eval3.2 ",c->cdr->cdr->cdr == 0);
    mu_assert("Error - eval3.3 ",!strcmp(c->car,"abc"));
    mu_assert("Error - eval3.4 ",(long)c->cdr->car == 9);
    mu_assert("Error - eval3.3 ",!strcmp(c->cdr->cdr->car,"xyz"));
    return 0;
}
static char *test_lambda1(){
    int parens;
    char str[] = "((lambda () (+ 1 3)))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - lambda1.1 ",(long)c->car == 4);
    return 0;
}
static char *test_lambda2(){
    int parens;
    char str[] = "(+ ((lambda () (+ 1 3)))  ((lambda () (+ 71 45))))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - lambda2.1 ",(long)c->car == 120);
    return 0;
}
 
static char *test_lambda3(){
    int parens;
    char str[] = "(+ ((lambda () (+ 1 3)))  ((lambda () (+ 71 ((lambda () (+ 9 8)))))))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - lambda3.1 ",(long)c->car == 92);
    return 0;
}
static char *test_lambda4(){
    int parens;
    char str[] = "((lambda () 255))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - lambda4.0 ",(long)c->car == 255);
    cons_print(c);
    return 0;
}
static char *test_def1(){
    int parens;
    char str[] = "(def five 5) five";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"def",cons(def,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - def1.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - def1.1 ",c->cdr->type == TYPE_INT);
    mu_assert("Error - def1.2 ",(long)c->cdr->car == 5);
    return 0;
}
static char *test_def2(){
    int parens;
    char str[] = "(def five (lambda () (+ 2 3))) (five)";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT environment = environment_new(0);
    environment_add(environment,"+",cons(adder,0,TYPE_NATIVE));
    environment_add(environment,"def",cons(def,0,TYPE_NATIVE));
    environment_add(environment,"atom",cons(is_atom,0,TYPE_NATIVE));
    Cell *c = eval(environment,p->cons);
    mu_assert("Error - def2.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - def2.1 ",c->cdr->type == TYPE_INT);
    mu_assert("Error - def2.2 ",(long)c->cdr->car == 5);
    cons_print(c);
    return 0;
}
static char *test_environment(){
    ENVIRONMENT environment1 = environment_new(0);
    ENVIRONMENT environment2 = environment_new(environment1);
    environment_add(environment1,"a","b");
    environment_add(environment1,"c","d");

    void *value = environment_search(environment1,"a");
    mu_assert("Error - test_environment1.0 ",!strcmp((char*)value,"b"));
    value = environment_search(environment2,"a");
    mu_assert("Error - test_environment1.1 ",!strcmp((char*)value,"b"));
    value = environment_search(environment2,"d");
    mu_assert("Error - test_environment1.2 ",value == 0);
    value = environment_search(environment1,"d");
    mu_assert("Error - test_environment1.3 ",value == 0);

    return 0;
}
 
char * all_tests() {
    mu_run_test(test_missing_rparen);
    mu_run_test(test_missing_lparen);
    mu_run_test(test_prepro1);
    mu_run_test(test_prepro2);
    mu_run_test(test_prepro3);
    mu_run_test(test_prepro4);
    mu_run_test(test_prepro5);
    mu_run_test(test_prepro6);
    mu_run_test(test_prepro7);
    mu_run_test(test_prepro8);
    mu_run_test(test_prepro9);
    mu_run_test(test_prepro10);
    mu_run_test(test_prepro11);
    mu_run_test(test_prepro12);
    mu_run_test(test_prepro13);
    mu_run_test(test_prepro14);
    mu_run_test(test_tokenize1);
    mu_run_test(test_tokenize2);
    mu_run_test(test_tokenize3);
    mu_run_test(test_parse1);
    mu_run_test(test_parse2);
    mu_run_test(test_parse3);
    mu_run_test(test_bst1);
    mu_run_test(test_eval1);
    mu_run_test(test_eval2);
    mu_run_test(test_eval3);
    mu_run_test(test_environment);
    mu_run_test(test_lambda1);
    mu_run_test(test_lambda2);
    mu_run_test(test_lambda3);
    mu_run_test(test_lambda4);
    mu_run_test(test_def1);
    mu_run_test(test_def2);
    if (tests_error != 0) {
        printf("%d errors.\n", tests_error);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return 0;
}
