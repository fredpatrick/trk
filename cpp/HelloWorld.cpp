
#include "HelloWorld.h"

#include <iostream>

HelloWorld::
HelloWorld() 
{
    std::cout << "HelloWorld::ctor\n";
}

HelloWorld::
~HelloWorld()
{
    std::cout << "HelloWorld::dtor\n";
}

void
HelloWorld::
doit()
{
     std::cout << "HelloWorld::doit: Hello World";
}
