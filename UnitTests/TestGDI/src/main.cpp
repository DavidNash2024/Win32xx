
/////////////////////////////
// main.cpp
//

#include "pch.h"
#include "test.h"
#include <iostream>


int main()
{
    // Initialise Win32++ (required to test LoadString).
    CWinApp myApp;

    RunTests();

    std::cout << "\nProgram complete. Press Enter.";
    std::cin.get();

    return 0;
}



