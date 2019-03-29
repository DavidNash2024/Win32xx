///////////////////////////////////////////////////
// A useful header file for console any app.
//
// Defines the following:
//  tcout           - a TCHAR version of std::cout
//


#ifndef _TCHAR_CONSOLE_SUPPORT_

#include <iostream>


// Add tcout (a TCHAR version of cout)
#ifdef UNICODE
	std::wostream& tcout = std::wcout;
#else
	std::ostream&  tcout = std::cout;
#endif


#endif // _TCHAR_CONSOLE_SUPPORT_
