/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of a basic Single Document Interface
    (SDI) class for the CommonDialogs sample program using the Win32++
    Windows interface classes, Copyright (c) 2005-2015 David Nash, under
    permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
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

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Declaration of the CApp class
    
*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/*******************************************************************************

    Class reference definitions                                         */

  // shorthand app and global references
#define theApp          TheApp()
#define theDoc          TheDoc()
#define theFrame        TheFrame()
#define theAppGlobal    theApp->GetAppGlobal()

/*============================================================================*/
    class 
CApp : public CWinApp                                                   /*

    This application's app class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
    public:
        CApp() : m_nCmdShow(0) {}
        ~CApp(){}

        BOOL        InitInstance(); // called from (CWinApp) Run()
        CAppGlobal  GetAppGlobal()      { return m_AppGlobal;};
        const CString& GetArcvFile() const {return m_AppGlobal.GetArcvPath();}
        int         GetCmdShow() const  {return m_nCmdShow;}
        CMainFrame& GetFrame()          {return m_Frame;}
        void        SetCmdShow(int c)   {m_nCmdShow = c;}

    private:
          // private data
        CString    m_sAppName;   // name of app: path minus directory
        CString    m_sAppDir;    // directory of this app
        CString    m_sAppPath;   // path to this app
        CString    m_sExeName;   // name of exe file, without extension
        CString    m_sArcvDir;   // directory holding the archive file
        CString    m_sArcvFile;  // initialization archive file name
        int        m_nCmdShow;   // WinMain() entry nCmdShow argument
        CMainFrame m_Frame;      // the main frame object
        CAppGlobal m_AppGlobal;
        
          // private functions
        ULONG   DatInt(const CString &);
        CString IntDat(ULONG);
};

/*============================================================================*/
    inline
CApp* TheApp()                                                            /*

    Global access to this application object.
*----------------------------------------------------------------------------*/
{
    return (CApp*)GetApp();
}

/*============================================================================*/
    inline
CMainFrame& TheFrame()                                                  /*

    Global access to the application document.
*----------------------------------------------------------------------------*/
{
    return (CMainFrame&)TheApp()->GetFrame();
}
/*============================================================================*/
    inline
CDoc& TheDoc()                                                          /*

    Global access to the application document.
*----------------------------------------------------------------------------*/
{
    return (CDoc&)TheFrame().GetDoc();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

