
/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include <iostream>
#include "Test.h"

//#include <vld.h>

int main()
{
    // Initialise Win32++ (required to test LoadString).
    CWinApp myApp;

    RunTests();

    std::cout << "\nProgram complete. Press Enter.";
    std::cin.get();

    return 0;
}



