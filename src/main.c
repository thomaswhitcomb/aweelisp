#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minunit.h>
#include <cell.h>
#include <native.h>

int main(int intc, char **argv){
    native_initialize();
    char *result = all_tests();

    return 0;
}
