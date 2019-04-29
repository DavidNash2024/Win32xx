/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of a skeleton Single Document
    Interface (SDI) CApp class, for use with the Win32++ Windows interface
    classes, Copyright (c) 2005-2015 David Nash, under permissions
    granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and  to the alterations,
    additions, deletions, and  other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and  any associated documentation composed by
    the author, to utilize this material, free of charge and  without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Special Conventions:

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement:
    The author would like to thank and acknowledge the advice, critical
    review, insight, and assistance provided by David Nash in the development
    of this work.
    
********************************************************************************

    Declaration of the CApp class

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/******************************************************************************/

#include "MainFrm.h"

/*============================================================================*/
    class 
CApp : public CWinApp                           /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp();
        virtual ~CApp();

        virtual BOOL        InitInstance();
        CMainFrame* TheFrame() {return &m_Frame;}

          // public data members
        CString   m_sAppName;   // name of app: minus directory and .exe
        CString   m_sAppDir;    // directory of this app
        CString   m_sAppPath;   // path to this app
        CString   m_sArcvDir;   // name of archive directory
        CString   m_sArcvFile;  // serialization file name

        static const CString m_sCompiled_on;   // compile date
        static const CString m_sCompiled_with; // compiler + system info
        static const CString m_sVersion;       // app version info
        static const CString m_sCredits;       // About box notices

        static CString m_sAboutStatement; // latest compile date
        
    protected:

    private:
          // private functions
        ULONG   DatInt(const CString &);
        CString IntDat(ULONG);

        CMainFrame m_Frame; // the mainframe

          // static constants
        static const CString m_months;
};

/*============================================================================*/
    inline
CApp& TheApp()                                                      /*

    Global function for access to this application.
*----------------------------------------------------------------------------*/
{
    return (CApp&)*GetApp();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

