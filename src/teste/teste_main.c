#include <stdio.h>
#include "teste_cppfunc.h"

extern void hello();


int main(){
    printf("Hello from C\n");
    hello();
    return 0;
}