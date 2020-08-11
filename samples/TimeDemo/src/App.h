/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|             Copyright (c) 2020, Robert C. Tausworthe, David Nash             |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description:  Declaration of the CApp class for this application
    using the Win32++ framework, Copyright (c) 2005-2020 David Nash, under
    permissions granted therein.

    The above copyright notice, as well as that of David Nash and Win32++,
    together with the respective permissionconditions shall be included in all
    copies or substantial portions of this material so copied, modified, merged,
    published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/******************************************************************************/

#include "MainFrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                   /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp();
        virtual ~CApp(){}

        virtual BOOL    InitInstance();
        CMainFrame*     TheFrame() {return &m_frame;}
        const CString&  GetAppName() const { return m_appName; }
        const CString&  GetArcFileName() const { return m_arcvFile; }
        const CString&  GetAboutStatement() const { return m_aboutStatement; }
        CString         MakeAppDataPath(const CString& subpath);

    private:
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

/*============================================================================*/
    inline
CApp* TheApp()                                                      /*

    Global function for access to this application.
*----------------------------------------------------------------------------*/
{
    return (CApp*)GetApp();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

