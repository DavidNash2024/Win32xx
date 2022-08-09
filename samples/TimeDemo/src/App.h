/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  Declaration of the CApp class for this application
    using the Win32++ framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/******************************************************************************/

#include "Mainfrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                   /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp();
        virtual ~CApp(){}

        virtual BOOL    InitInstance();
        const CString&  GetAppName() const { return m_appName; }
        const CString&  GetArcFileName() const { return m_arcvFile; }
        const CString&  GetAboutStatement() const { return m_aboutStatement; }
        CString         MakeAppDataPath(const CString& subpath);

    private:
        CApp(const CApp&);                // Disable copy construction
        CApp& operator = (const CApp&);   // Disable assignment operator

          // private functions
        ULONG       DatInt(const CString &);
        CString     IntDat(ULONG);

        CMainFrame  m_frame;            // the mainframe
        CString     m_appName;          // name of app: minus directory and .exe
        CString     m_appDir;           // directory of this app
        CString     m_appPath;          // path to this app
        CString     m_arcvDir;          // name of archive directory
        CString     m_arcvFile;         // serialization file name
        CString     m_aboutStatement;   // latest compile date
        CString     m_months;           // month enumeration
        CString     m_compiledWith;     // compiler + system info
        CString     m_version;          // app version info
        CString     m_credits;          // About box notices
        CString     m_archiveFileType;  // Archive type

        static const CString m_compiledOn;   // latest compile date

};

/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

