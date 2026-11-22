
/////////////////////////////
// main.cpp
//

#include "pch.h"      // includes the Win32++ library.
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



