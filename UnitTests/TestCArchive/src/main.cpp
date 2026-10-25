/////////////////////////////
// main.cpp
//

#include "pch.h"             // Include the entire Win32++ library.
#include <iostream>
#include "Tests.h"

using namespace std;

int main()
{
    cout << "\n*** CArchive Tests begin\n\n";

    bool passed = DoTests();
    if (passed) cout << "\nPassed: All Tests\n";
    else        cout << "\n*** Some tests failed ***\n";

    cout << "\n*** CArchive Tests complete\n\n";

    cout << "Press Enter to continue...";
    cin.get();

    return 0;
}