/////////////////////////////
// ContextHelp.cpp
//

//  This class creates a window to display a .chm help file.
//
//  CContextHelp implements the following functions:
//  AddHelpTopic.       Adds a <UINT, CString> pair to a map of known topics.
//  CreateHtmlHelp.     Creates a HtmlHelp window.
//  ShowHelpTopic.      Creates a HtmlHelp window and displays the specified topic.
//  GetHelpFilePath.    Retrieves the pathname of the chm help file.
//  SetHelpFilePath     Set the pathname of the chm help file
//
//  The .chm help file is typically placed in a subdirectory of the user's
//  APPDATA environmental path.
//
//  Be sure to load the htmlhelp.h and htmlhelp.lib precisely as
//  described in Adding Compiled HTML Help to MinGW Win32 Apps.html.
//  Add the link to the libhtmlhelp.a to use this function
//  (linker parameter -lhtmlhelp in DevC++). Ignore the compiler
//  warning message ".drectve `-defaultlib:uuid.lib ' unrecognized".
//  For other compilers, add the htmlhelp.lib to the linker list.
//
//  How to use CContexHelp
//  1) Use SetHelpFilePath to specify the chm help file.
//  2) Use AddHelpTopic to add a <UINT, CString> pair to the map of known topics.
//  3) Use ShowHelpTopic to display the help for a specified topic.
//
//  Acknowledgement:
//    I wish to thank  Robert C. Tausworthe for providing the original
//    code and for his ongoing assistance in developing this sample.


#include "stdafx.h"
#include <htmlhelp.h>  // load after windows.h to get system #defines

#include "ContextHelp.h"
#include "resource.h"


////////////////////////////////////
// CContextHelp function definitions
//

// Constructor.
CContextHelp::CContextHelp()
{
}

// Destructor
CContextHelp::~CContextHelp()
{
}

// Add the (control-id, string topic) pair to the help topic table.
// Assert if a duplicate id entered.
void CContextHelp::AddHelpTopic(UINT id, LPCTSTR topic)
{
    assert(id);
    assert(topic);
    std::map<UINT, CString>::const_iterator m;

    // Ensure this isn't a duplicate entry
    m = m_helpTopics.find(id);
    assert(m == m_helpTopics.end());

    // Add the topic to the map
    m_helpTopics.insert(std::make_pair(id, topic));
}

// Creates the HtmlHelp window, and binds this object to its HWND.
// hwndCaller: The handle (HWND) of the window calling HtmlHelp, typically ::GetDesktopWindow().
//              The help window is owned by this window.
// string: Depending on the uCommand value, specifies the file path to either a compiled help (.chm) file,
//            or a topic file within a specified help file.
// command: Specifies the command to complete, typically HH_DISPLAY_TOPIC.
//            Refer to the MSDN documentation for possible uCommand values.
// data:   Specifies any data that may be required, based on the value of the uCommand parameter.
HWND CContextHelp::CreateHtmlHelp(HWND hwndCaller, LPCTSTR string, UINT command, DWORD data)
{
    // Prepare this CWnd for possible re-use
    if (*this != 0) Detach();

    // Create the HtmlHelp window
    HWND hWnd = HtmlHelp(hwndCaller, string, command, data);

    // Throw an exception if the window wasn't created
    if (hWnd == 0) throw CWinException(_T("Failed to initiate Context Help"));

    // Bind the hWnd to this CWnd object
    Attach(hWnd);

    return hWnd;
}

// Identifies the window from the cursor position and returns its ID.
UINT CContextHelp::GetIDFromCursorPos() const
{
    UINT id = 0;
    CPoint pt = GetCursorPos();
    HWND hCtrl = ::WindowFromPoint(pt);

    if (hCtrl != 0)
    {
        id = ::GetDlgCtrlID(hCtrl);
    }

    return id;
}

// Display the application guide topic corresponding to the numeric
// identifier id, if present in the help table, and if the topic exists
// in the guide.
void CContextHelp::ShowHelpTopic(UINT id)
{
    std::map<UINT, CString>::const_iterator m;

    // Find the CString mapped to this UINT
    m = m_helpTopics.find(id);
    CString topic = (m != m_helpTopics.end()) ? m->second : CString(_T("FeatureNotDescribed"));

    ShowHelpTopic(topic);
}

// Display the application guide topic, if present, or show the topic
// in a message box if there is no such topic in the guide.
void CContextHelp::ShowHelpTopic(LPCTSTR topic)
{
    CString topic_url = (topic[0] == 0) ? CString(_T("")) : _T("::/") + CString(topic) + _T(".htm");
    CString seek_url = m_helpFilePath + topic_url;

    try
    {
        CreateHtmlHelp(::GetDesktopWindow(), seek_url.c_str(), HH_DISPLAY_TOPIC, 0);
    }

    catch (const CWinException&)
    {
        // if no help guide topic was opened, show that this happened
        CString s;
        CString fmt = _T("Help topic could not be located:\n\n%s%s");
        CString add = (m_helpFilePath.IsEmpty() ? _T("\n\nNo help guide exists.")
            : _T("\n\nMake sure the .chm file is in the .exe directory."));

        s.Format(fmt, topic, add.c_str());
        ::MessageBox(NULL, s, _T("Information"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    }
}






