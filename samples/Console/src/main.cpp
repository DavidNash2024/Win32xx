//////////////////////////////////
// main.cpp


#include "stdafx.h"				// Includes the Win32++ header files
#include "tchar_console.h"		// Adds tcout (a TCHAR version of std::cout)


using namespace std;


int main()
{
	// Demonstrate the use of CString with tcout
	CString text = "Hello world";
	tcout << text.c_str() << endl;
	
	return 0;
}
