#include "objTest.h"
    

Test::Test(){
    auto terminal = TextMode();
    terminal.println("Create a test object");
};

void Test::printMe(){
    auto terminal = TextMode();
    terminal.println("testObject.printMe()\n");
}

Test::~Test(){
    auto terminal = TextMode();
    terminal.println("Destroy testObject! Bye bye\n");
}
