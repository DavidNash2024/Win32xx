/* (13-Sep-2015) [Tab/Indent: 8/8][Line/Box: 80/74]            (GlobalUtils.h) *
********************************************************************************
|                                                                              |
|                     Copyright (c) 2017, Robert C. Tausworthe                 |
|                             All Rights Reserved.                             |
|                                                                              |
*==============================================================================*

    Global utility functions and constants declaration file. This header
    file contains a number of generally useful macro functions and constants
    that do not belong to any class.  These have been adapted for use in
    applications utilizing the Win32++ framework, Copyright (c) 2005-2015
    David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
        treatment of material that may have been extracted from cited sources.
    Unaltered portions of those materials retain their original copyright
    status. The author's additions and modifications may be used or altered
    by others for non-commercial use only without permission.  However,
    citation of the author's work should be included in such usages.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Global C++ Utility Functions Definitions

*******************************************************************************/

#ifndef GLOBUTIL_H_IS_LOADED
#define GLOBUTIL_H_IS_LOADED

#include <io.h>
#include "stdafx.h"

/*******************************************************************************

        STANDARD MACROS

    For readability: Note: some of these (commented out below) are ISO
    standard C++ reserved words that were not yet recognized in Visual
    C++ 6.0, but are recognized by minGW.               */

#ifndef AND
#define AND &&
#endif

#ifdef ISO_646_NOT_SUPPORTED
#ifndef and
#define and &&
#endif

#ifndef is
#define is  ==
#endif

#ifndef or
#define or  ||
#endif

#ifndef not
#define not !
#endif

#ifndef xor
#define xor ^
#endif

#endif // ISO_646_NOT_SUPPORTED

#ifndef IS
#define IS  ==
#endif

#ifndef ISNT
#define ISNT    !=
#endif

#ifndef isnt
#define isnt    !=
#endif

#ifndef OR
#define OR  ||
#endif

#ifndef NOT
#define NOT !
#endif

#ifndef MOD
#define MOD %
#endif

#ifndef mod
#define mod %
#endif

#ifndef XOR
#define XOR ^
#endif

#ifndef VERIFY
#define VERIFY(expr)    {if ((expr) is 0) throw;}
#endif

  // CDC unit conversions
static const double TWIPS_PER_IN    =   1440.;  // converts inches to twips
static const double PTS_PER_TWIP    =     20.;  // converts points to twips
static const double TWIPS_PER_MM    = 56.6929;  // convert mm to twips
static const double MM_PER_IN       = 25.4000;  // ISO standard, exact
static const double IN_PER_MM       =  0.0400;

#ifndef EOLN
#define EOLN            _T("\n")    // end of line string
#endif

#ifndef EOLNC
#define EOLNC           _T('\n')    // end of line TCHAR
#endif

  // static constants
static const TCHAR  cEOL        = _TCHAR('\n');
static const TCHAR  cNul        = _TCHAR('\0');
static const TCHAR  cPathSeparator  = _TCHAR('\\');
static const TCHAR  cLPathSeparator = _TCHAR('/');
static const TCHAR  cExtSeparator   = _TCHAR('.');
static const TCHAR  cVolSeparator   = _TCHAR(':');
static const TCHAR  sEOL[]      = _T("\n");
static const TCHAR  sSvrBegin[] = _T("\\\\");
static const TCHAR  sPathSeparator[] = _T("\\");
static const TCHAR  sLPathSeparator[] = _T("/");
static const TCHAR  sExtSeparator[] = _T(".");
static const TCHAR  sVolSeparator[] = _T(":");
static const TCHAR  sDblQuote[] = _T("\"");
static const TCHAR  sSglQuote[] = _T("'");
static const TCHAR  sSglQuoteRepl[] = _T(" ");
static const TCHAR  sEolChars[] = _T("\n\r");
static const TCHAR  sEmpty[]    = _T("");
static const TCHAR  sAllFiles[] = _T("*.*");
static const TCHAR  sWhitespace[]   = _T(" \t\n\r\f\v");
static const UINT   nSearchFail = static_cast<UINT>(-1);
static const UINT   nMaxSize_t  = static_cast<UINT>(-1);

//static const UINT CTL_OFFSET  = 0x100;
//static const UINT CTL_DOWN    = CTL_OFFSET + VK_DOWN;
//static const UINT CTL_END = CTL_OFFSET + VK_END;
//static const UINT CTL_HOME    = CTL_OFFSET + VK_HOME;
//static const UINT CTL_LEFT    = CTL_OFFSET + VK_LEFT;
//static const UINT CTL_RIGHT   = CTL_OFFSET + VK_RIGHT;
//static const UINT CTL_UP  = CTL_OFFSET + VK_UP;

  // file access functions
inline BOOL PathExists(LPCTSTR path)    { return (::_taccess(path, 0) == 0);}
inline BOOL PathReadable(LPCTSTR path)  { return (::_taccess(path, 4) == 0);}
inline BOOL PathWritable(LPCTSTR path)  { return (::_taccess(path, 2) == 0);}
inline BOOL PathReadWrite(LPCTSTR path) { return (::_taccess(path, 6) == 0);}

  // Miscellaneous math functions
#ifndef SGN
#define SGN(x)  (x > 0 ? 1.0 : (x < 1 ? -1.0 : 0.))
#endif

#ifndef ABS
#define ABS(x)  (x > 0 ? x : (x < 1 ? -(x) : 0.))
#endif

#ifndef SWAP
#define SWAP(T, g, h) {T y = (g); (g) = (h); (h) = y;}
#endif

#ifndef SIGN  // give a the sign of b
#define SIGN(a, b) ((b) >= 0 ? fabs(a) : -fabs(a))
#endif

  // Runtime verification macro
#ifndef POSIT   // assert expression, show diagnostic message, throw exception
#define POSIT(expression, exception) \
{ \
    if (!(expression)) \
    { \
        CString s; \
        s.Format("Assertion failed: %s\n   infile: %s\n   line %d", \
            #expression, __FILE__, __LINE__); \
        ::MessageBox(NULL, s, _T("Exception"), MB_OK | \
            MB_ICONEXCLAMATION | MB_TASKMODAL); \
        throw (exception); \
    } \
}

#endif

  // MessageBox modes
    inline void
ErrorMessageBox(CString msg)    {::MessageBox(NULL, msg, _T("Error"), MB_OK |
                    MB_ICONEXCLAMATION | MB_TASKMODAL);}
    inline void
InfoMessageBox(CString msg) {::MessageBox(NULL, msg, _T("Information"),
                    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);}
    inline BOOL
QuestionMessageBox(CString msg) { return (::MessageBox(NULL, msg, _T("Question"),
                    MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL)
                    == IDYES);}
    inline void
StopMessageBox(CString msg) {::MessageBox(NULL, msg, _T("Stop"),
                    MB_OK | MB_ICONSTOP | MB_TASKMODAL);}
    inline void
YesNoMessageBox(CString msg)    {::MessageBox(NULL, msg, _T("Question"),
                    MB_YESNO |MB_ICONQUESTION | MB_TASKMODAL);}
    inline void
YesNoCancelBox(CString msg) {::MessageBox(NULL, msg, _T("Question"),
                    MB_ICONQUESTION | MB_YESNOCANCEL);}
    inline void
ExceptionMessageBox(CString msg){::MessageBox(NULL, msg, _T("Exception"),
                    MB_OK | MB_ICONSTOP | MB_TASKMODAL);}

  // To avoid compiler warnings re: unused parameter
#define UNUSED_PARAMETER(p) {UINT iTemp = sizeof(p); iTemp++;}

  // process suspension values
enum    runstat     {CONTINUE, PAUSE, ABORT};

/*----------------------------------------------------------------------------*/
#endif // GLOBUTIL_H_IS_LOADED

