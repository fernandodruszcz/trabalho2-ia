#include <iostream>
#include "teste_cppfunc.h"
using namespace std;

extern "C" void hello();

void hello(){
    cout << "Hello from C++\n"; 
}

// int main(){
//     hello();
//     return 0;
// }