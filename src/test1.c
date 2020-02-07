#include <minunit.h>
#include <stdio.h>
#include <string.h>
#include <parse.h>
#include <tokenize.h>
#include <bst.h>
#include <eval.h>
#include <environment.h>
#include <native.h>

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
    mu_assert("Error - test_parse2.1",p->cell->type == TYPE_LIST);
    Cell *list = p->cell->datum;
    mu_assert("Error - test_parse2.2",!strcmp(list->datum,"hi"));
    mu_assert("Error - test_parse2.3",!strcmp(list->next->datum,"bye"));
    return 0;
}
static char *test_parse3(){
    char str[] = "(spic (+ 2 3) span)";
    int parens;
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    mu_assert("Error - test_parse3.1",p->cell->type == TYPE_LIST);
    Cell *list = p->cell->datum;
    mu_assert("Error - test_parse3.2",!strcmp(list->datum,"spic"));
    mu_assert("Error - test_parse3.3",list->next->type == TYPE_LIST);
    mu_assert("Error - test_parse3.4",list->next->next->type == TYPE_SYMBOL);
    mu_assert("Error - test_parse3.5",!strcmp(list->next->next->datum,"span"));
    list = (Cell *)list->next->datum;
    mu_assert("Error - test_parse3.6",!strcmp(list->datum,"+"));
    mu_assert("Error - test_parse3.7",(long) list->next->datum == 2);
    mu_assert("Error - test_parse3.8",(long) list->next->next->datum == 3);
    mu_assert("Error - test_parse3.9",list->next->next->next == 0);
    return 0;
}
static char *test_parse4(){
    int parens;
    char str[] = "()";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    mu_assert("Error - test_parse4.1",p->cell->type == TYPE_LIST);
    mu_assert("Error - test_parse4.2",p->cell->datum == 0);
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
    mu_assert("Error - test_bst1.0 ",result == (void *)31);
    result = bst_search(bst,(void *)25);
    mu_assert("Error - test_bst1.1 ",result == (void *)25);
    result = bst_search(bst,(void *)18);
    mu_assert("Error - test_bst1.2 ",result == (void *)18);
    result = bst_search(bst,(void *)1000);
    mu_assert("Error - test_bst1.3 ",result == 0);
    return 0;
}

static char *test_bst2(){
    BST *bst = bst_new(myintcmp);
    bst_insert(bst,(void *)25,(void *)25);
    bst_insert(bst,(void *)15,(void *)15);
    bst_insert(bst,(void *)25,(void *)50);
    bst_insert(bst,(void *)24,(void *)24);
    void *result = bst_search(bst,(void *)25);
    mu_assert("Error - test_bst2 ",result == (void *)50);
    return 0;
}
static char *test_eval1(){
    //char str[] = "(+ 2 (+ 5 7 3))";
    char str[] = "(+ (+ 2 (+ 5 7 3)) 10)";
    int parens;
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - eval1.1 ",c != 0);
    mu_assert("Error - eval1.2 ",c -> next == 0);
    mu_assert("Error - eval1.3 ",(long) c -> datum == 27);
    return 0;
}
static char *test_eval2(){
    int parens = 0;
    char str[] = "(atom 5)";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - eval2.1 ",c != 0);
    mu_assert("Error - eval2.2 ",c -> next == 0);
    mu_assert("Error - eval2.3 ",c -> datum == 0);
    mu_assert("Error - eval2.4 ",c -> type == TYPE_TRUE);
    return 0;
}
static char *test_eval3(){
    char str[] = "abc (+ 4 5) xyz)";
    Cell *c = eval_main(str);
    mu_assert("Error - eval3.1 ",c != 0);
    mu_assert("Error - eval3.2 ",!strcmp(c->datum,"abc"));
    mu_assert("Error - eval3.3 ",(long)c->next->datum == 9);
    mu_assert("Error - eval3.3 ",!strcmp(c->next->next->datum,"xyz"));
    return 0;
}
static char *test_def1(){
    int parens;
    char str[] = "(def five 5) five";
    Cell *c = eval_main(str);
    mu_assert("Error - def1.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - def1.1 ",c->next->type == TYPE_INT);
    mu_assert("Error - def1.2 ",(long)c->next->datum == 5);
    return 0;
}
static char *test_def2(){
    char str[] = "(def five (lambda () (+ 2 3))) (five)";
    Cell *c = eval_main(str);
    mu_assert("Error - def2.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - def2.1 ",c->next->type == TYPE_INT);
    mu_assert("Error - def2.2 ",(long)c->next->datum == 5);
    return 0;
}
static char *test_environment(){
    ENVIRONMENT *environment1 = environment_new(0);
    ENVIRONMENT *environment2 = environment_new(environment1);
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
static char *test_lambda1(){
    int parens;
    char str[] = "((lambda () (+ 1 3)))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - lambda1.1 ",(long)c->datum == 4);
    return 0;
}
static char *test_lambda2(){
    int parens;
    char str[] = "(+ ((lambda () (+ 1 3)))  ((lambda () (+ 71 45))))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - lambda2.0 ",c->type == TYPE_INT);
    mu_assert("Error - lambda2.1 ",(long)c->datum == 120);
    return 0;
}
 
static char *test_lambda3(){
    int parens;
    char str[] = "(+ ((lambda () (+ 1 3)))  ((lambda () (+ 71 ((lambda () (+ 9 8)))))))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - lambda3.1 ",(long)c->datum == 92);
    return 0;
}
static char *test_lambda4(){
    int parens;
    char str[] = "((lambda () 255))";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - lambda4.0 ",(long)c->datum == 255);
    return 0;
}
static char *test_lambda5(){
    int parens;
    char str[] = "((lambda (a b c) (+ a b c)) 1 2 3 )";
    char **array = tokenize(str,&parens);
    Parser *p = parse(array);
    ENVIRONMENT *environment = environment_new(0);
    Cell *c = eval(environment,p->cell);
    mu_assert("Error - lambda5.0 ",c->type == TYPE_INT);
    mu_assert("Error - lambda5.1 ",(long) c->datum == 6);
    return 0;
}
static char *test_lambda6(){
    char str[] = "(def add (lambda (x y) (+ x y))) (add 56 7)";
    Cell *c = eval_main(str);
    mu_assert("Error - lambda6.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - lambda6.1 ",(long)c->next->datum == 63);
    return 0;
}
static char *test_lambda7(){
    char str[] = "(def cons (lambda (x y) (lambda (m) (m x y)))) (def car (lambda (z) (z (lambda (p q) p)))) (def cdr (lambda (z) (z (lambda (p q) q)))) (car (cons 99 202)) (cdr (cons 99 202))"; 
    Cell *c = eval_main(str);
    mu_assert("Error - lambda7.0 ",c->type == TYPE_TRUE);
    mu_assert("Error - lambda7.1 ",c->next->type == TYPE_TRUE);
    mu_assert("Error - lambda7.2 ",c->next->next->type == TYPE_TRUE);
    mu_assert("Error - lambda7.3 ",c->next->next->next->type == TYPE_INT);
    mu_assert("Error - lambda7.4 ",c->next->next->next->next->type == TYPE_INT);
    mu_assert("Error - lambda7.5 ",(long) c->next->next->next->datum == 99);
    mu_assert("Error - lambda7.6 ",(long) c->next->next->next->next->datum == 202);
    return 0;
}
static char *test_lambda8(){
    char str[] = "(def cons (lambda (x y) (lambda (m) (m x y)))) (def car (lambda (z) (z (lambda (p q) p)))) (def cdr (lambda (z) (z (lambda (p q) q)))) (cdr (cons 99 202))"; 
    eval_main(str);
    return 0;
}
static char *test_atom1(){
    char str[] = "(atom 5)";
    Cell *c = eval_main(str);
    mu_assert("Error - atom1.0 ",c->type == TYPE_TRUE);
    return 0;
}
static char *test_atom2(){
    char str[] = "(atom abc)";
    Cell *c = eval_main(str);
    mu_assert("Error - atom2.0 ",c->type == TYPE_TRUE);
    return 0;
}
static char *test_atom3(){
    char str[] = "(atom (list))";
    Cell *c = eval_main(str);
    mu_assert("Error - atom3.0 ",c->type == TYPE_LIST);
    mu_assert("Error - atom3.1 ",c->datum == 0);
    return 0;
}
static char *test_atom4(){
    char str[] = "(atom (list 1 2 3))";
    Cell *c = eval_main(str);
    mu_assert("Error - atom4.0 ",c->type == TYPE_LIST);
    mu_assert("Error - atom4.1 ",c->datum == 0);
    return 0;
}
static char *test_quote1(){
    char str[] = "(quote 5)";
    Cell *c = eval_main(str);
    mu_assert("Error - quote1.0 ",(long)c->datum == 5);
    return 0;
}
static char *test_quote2(){
    char str[] = "(quote (atom abc))";
    Cell *c = eval_main(str);
    mu_assert("Error - quote2.0 ",c->type == TYPE_LIST);
    mu_assert("Error - quote2.1 ",((Cell *)c->datum)->type == TYPE_SYMBOL);
    c = (Cell *)c->datum;
    mu_assert("Error - quote2.2 ",!strcmp(c->datum, "atom"));
    c = (Cell *)c->next;
    mu_assert("Error - quote2.2 ",!strcmp(c->datum, "abc"));
    c = (Cell *)c->next;
    mu_assert("Error - quote2.3 ",c==0);
    return 0;
}
static char *test_cell_clone1(){
    char str[] = "5";
    Cell *c = eval_main(str);
    Cell *clone = cell_clone(c);
    cell_print(clone);
    printf("equal: %i\n",cell_equal(c,clone));
    return 0;
}
static char *test_cell_clone2(){
    char str[] = "(quote (5))";
    Cell *c = eval_main(str);
    Cell *clone = cell_clone(c);
    cell_print(clone);
    printf("equal: %i\n",cell_equal(c,clone));
    return 0;
}
static char *test_cell_clone3(){
    char str[] = "(quote (5 4 3 2 (+ 2 3)))";
    Cell *c = eval_main(str);
    Cell *clone = cell_clone(c);
    cell_print(clone);
    printf("equal: %i\n",cell_equal(c,clone));
    return 0;
}
static char *test_cell_clone4(){
    char str[] = "(quote (lambda (p1 p2) (+ 2 3)))";
    Cell *c = eval_main(str);
    Cell *clone = cell_clone(c);
    cell_print(clone);
    printf("equal: %i\n",cell_equal(c,clone));
    return 0;
}
static char *test_cell_clone5(){
    char str[] = "(quote (def abc (lambda (p1 p2) (+ (+ 6 7 9) 3))))";
    Cell *c = eval_main(str);
    Cell *clone = cell_clone(c);
    cell_print(clone);
    printf("equal: %i\n",cell_equal(c,clone));
    return 0;
}
static char *test_cons1(){
    char str[] = "(cons 5 (quote (10)))";
    Cell *c = eval_main(str);
    mu_assert("Error - cons1.0 ",c->type == TYPE_LIST);
    mu_assert("Error - cons1.1 ",(long)((Cell *)c->datum)->datum == 5);
    mu_assert("Error - cons1.2 ",(long)((Cell *)c->datum)->next->datum == 10);
    mu_assert("Error - cons1.3 ",((Cell *)c->datum)->next->next== 0);
    return 0;
}
static char *test_cons2(){
    char str[] = "(cons (quote (abc def)) (quote (10 47)))";
    Cell *c = eval_main(str);
    mu_assert("Error - cons2.0 ",c->type == TYPE_LIST);
    cell_print(c);
    return 0;
}
static char *test_cons3(){
    char str[] = "(cons (quote (abc def)) (quote ()))";
    Cell *c = eval_main(str);
    mu_assert("Error - cons3.0 ",c->type == TYPE_LIST);
    cell_print(c);
    return 0;
}
static char *test_car1(){
    char str[] = "(car (quote ()))";
    Cell *c = eval_main(str);
    mu_assert("Error - car1.0 ",c->type == TYPE_NIL);
    return 0;
}
static char *test_car2(){
    char str[] = "(car (quote (1)))";
    Cell *c = eval_main(str);
    mu_assert("Error - car2.0 ",c->type == TYPE_INT);
    mu_assert("Error - car2.1 ",(long)c->datum == 1);
    return 0;
}
static char *test_car3(){
    char str[] = "(car (quote ((1 2) 3 4)))";
    Cell *c = eval_main(str);
    mu_assert("Error - car3.0 ",c->type == TYPE_LIST);
    mu_assert("Error - car3.1 ",(long)(((Cell *)c->datum)->datum) == 1);
    mu_assert("Error - car3.2 ",(long)(((Cell *)c->datum)->next->datum) == 2);
    return 0;
}
static char *test_cdr1(){
    char str[] = "(cdr (quote ()))";
    Cell *c = eval_main(str);
    mu_assert("Error - cdr1.0 ",c->type == TYPE_LIST);
    mu_assert("Error - cdr1.1 ",(long) c->datum == 0);
    return 0;
}
static char *test_cdr2(){
    char str[] = "(cdr (quote (1)))";
    Cell *c = eval_main(str);
    mu_assert("Error - cdr2.0 ",c->type == TYPE_LIST);
    mu_assert("Error - cdr2.1 ",c->datum == 0);
    return 0;
}
static char *test_cdr3(){
    char str[] = "(cdr (quote ((1 2) 3 4)))";
    Cell *c = eval_main(str);
    mu_assert("Error - cdr3.0 ",c->type == TYPE_LIST);
    mu_assert("Error - cdr3.1 ",((long)((Cell *)c->datum)->datum) == 3);
    mu_assert("Error - cdr3.1 ",(long)((Cell *)c->datum)->next->datum == 4);
    return 0;
}
static char *test_car_cdr1(){
    char str[] = "(car (cdr (quote ((1 2) 3 4))))";
    Cell *c = eval_main(str);
    mu_assert("Error - car_cdr1.0 ",c->type == TYPE_INT);
    mu_assert("Error - car_cdr1.1 ",(long)c->datum == 3);
    return 0;
}
static char *test_list1(){
    char str[] = "(list 1 2 3)";
    Cell *c = eval_main(str);
    mu_assert("Error - test1.0 ",c->type == TYPE_LIST);
    mu_assert("Error - test1.1 ",(long)((Cell *)c->datum)->datum == 1);
    c = ((Cell *)c->datum)->next;
    mu_assert("Error - test1.2 ",(long)(c->datum) == 2);
    c = c->next;
    mu_assert("Error - test1.3 ",(long)(c->datum) == 3);
    c = c->next;
    mu_assert("Error - test1.4 ",c == 0);
    return 0;
}
static char *test_list2(){
    char str[] = "(list)";
    Cell *c = eval_main(str);
    mu_assert("Error - test2.0 ",c->type == TYPE_LIST);
    mu_assert("Error - test2.1 ",c->datum == 0);
    return 0;
}
static char *test_list3(){
    char str[] = "(list (list))";
    Cell *c = eval_main(str);
    mu_assert("Error - test3.0 ",c->type == TYPE_LIST);
    mu_assert("Error - test3.1 ",(long)((Cell *)c->datum)->type == TYPE_LIST);
    c = (Cell *)c->datum;
    mu_assert("Error - test3.2 ",c->datum == 0);
    return 0;
}
static char *test_eq1(){
    char str[] = "(eq 1 1)";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq1.0 ",c->type == TYPE_TRUE);
    return 0;
}
static char *test_eq2(){
    char str[] = "(eq abc abc)";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq2.0 ",c->type == TYPE_TRUE);
    return 0;
}
static char *test_eq3(){
    char str[] = "(eq (list) (list))";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq3.0 ",c->type == TYPE_TRUE);
    return 0;
}
static char *test_eq4(){
    char str[] = "(eq (list) 1)";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq4.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq4.1 ",c->datum == 0);
    return 0;
}
static char *test_eq5(){
    char str[] = "(eq (list 1) (list))";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq5.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq5.1 ",c->datum == 0);
    return 0;
}
static char *test_eq6(){
    char str[] = "(eq 1 2 3)";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq6.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq6.1 ",c->datum == 0);
    return 0;
}
static char *test_eq7(){
    char str[] = "(eq 1 )";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq7.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq7.1 ",c->datum == 0);
    return 0;
}
static char *test_eq8(){
    char str[] = "(eq abc 123 )";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq8.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq8.1 ",c->datum == 0);
    return 0;
}
static char *test_eq9(){
    char str[] = "(eq 2 3 )";
    Cell *c = eval_main(str);
    mu_assert("Error - testeq8.0 ",c->type == TYPE_LIST);
    mu_assert("Error - testeq8.1 ",c->datum == 0);
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
    mu_run_test(test_parse4);
    mu_run_test(test_bst1);
    mu_run_test(test_bst2);
    mu_run_test(test_eval1);
    mu_run_test(test_eval2);
    mu_run_test(test_eval3);
    mu_run_test(test_environment);
    mu_run_test(test_def1);
    mu_run_test(test_def2);
    mu_run_test(test_lambda1);
    mu_run_test(test_lambda2);
    mu_run_test(test_lambda3);
    mu_run_test(test_lambda4);
    mu_run_test(test_lambda5);
    mu_run_test(test_lambda6);
    mu_run_test(test_lambda7);
    mu_run_test(test_lambda8);
    mu_run_test(test_atom1);
    mu_run_test(test_atom2);
    mu_run_test(test_atom3);
    mu_run_test(test_atom4);
    mu_run_test(test_quote1);
    mu_run_test(test_quote2);
    mu_run_test(test_cell_clone1);
    mu_run_test(test_cell_clone2);
    mu_run_test(test_cell_clone3);
    mu_run_test(test_cell_clone4);
    mu_run_test(test_cell_clone5);
    mu_run_test(test_cons1);
    mu_run_test(test_cons2);
    mu_run_test(test_cons3);
    mu_run_test(test_car1);
    mu_run_test(test_car2);
    mu_run_test(test_car3);
    mu_run_test(test_cdr1);
    mu_run_test(test_cdr2);
    mu_run_test(test_cdr3);
    mu_run_test(test_car_cdr1);
    mu_run_test(test_list1);
    mu_run_test(test_list2);
    mu_run_test(test_list3);
    mu_run_test(test_eq1);
    mu_run_test(test_eq2);
    mu_run_test(test_eq3);
    mu_run_test(test_eq4);
    mu_run_test(test_eq5);
    mu_run_test(test_eq6);
    mu_run_test(test_eq7);
    mu_run_test(test_eq8);
    mu_run_test(test_eq9);
    if (tests_error != 0) {
        printf("%d errors.\n", tests_error);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return 0;
}
