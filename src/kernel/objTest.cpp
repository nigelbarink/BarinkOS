#include "objTest.h"


Test::Test(){
    kterm_writestring("Create a test object\n");
};

void Test::printMe(){
    kterm_writestring("testObject.printMe()\n");
}

Test::~Test(){
    kterm_writestring("Destroy testObject! Bye bye\n");
}
