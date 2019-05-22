/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of a basic Single Document Interface (SDI) 
    CApp class for the ScrollWin demo application using the Win32++ Windows 
    interface classes, Copyright c) 2005-2017 David Nash, under permissions 
    granted therein.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/*============================================================================*/
    class 
CApp : public CWinApp                                                       /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp();
        virtual ~CApp();

        virtual BOOL    InitInstance();
        const CString&  GetAboutBoxInfo() const { return m_sAboutBoxInfo;}
        const CString&  GetArcvPath()     const { return m_sArcvPath;}
        CMainFrame&     GetFrame()              { return m_Frame;}
        UINT            GetMaxMRUSlots() const  { return m_nMaxMRUSlots;}
        CMainFrame&     TheFrame()              { return m_Frame;}

    private:
          // private functions
        CString  MakeAppDataPath(const CString& subpath) const;
        ULONG    DatInt(LPCTSTR) const;
        CString  IntDat(ULONG) const;

          // private data
        CString     m_sArcvPath;  // archive file name
        CString     m_sAboutBoxInfo; // credits
        CMainFrame  m_Frame;      // the main frame object
        UINT        m_nMaxMRUSlots;

};

/*============================================================================*/
    inline
CApp& TheApp()                                                              /*

    Handy global function
*----------------------------------------------------------------------------*/
{
    return (CApp&)*GetApp();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

