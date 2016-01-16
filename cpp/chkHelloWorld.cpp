#include <iostream>

#include "HelloWorld.h"

int main()
{
    HelloWorld* hw;
    hw = new HelloWorld;
    hw->doit();
    std::cout << "main: Hello World!\n";
    delete hw;
}
