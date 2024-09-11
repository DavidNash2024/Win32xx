/////////////////////////////
// main.cpp
//

#include <iostream>
#include "wxx_cstring.h"        // Includes CString from Win32++

using namespace std;

int main()
{
    // Demonstrate the use of CString with wcout.
    CString text = "Hello world";
    wcout << text.c_str() << endl;

    return 0;
}
