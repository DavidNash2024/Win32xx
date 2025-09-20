/////////////////////////////
// main.cpp
//

#include <iostream>
#include "wxx_cstring.h"        // Includes CString from Win32++.
// #include "wxx_socket.h"      // Includes network support from Win32++.
// #include "stdafx.h"          // Include the entire Win32++ library.

using namespace std;

int main()
{
    // Demonstrate the use of CString with wcout.
    CString text = "Hello world";
    wcout << text.c_str() << endl;

    return 0;
}
