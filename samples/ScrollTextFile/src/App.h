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
        CString&        GetAboutBoxInfo()   { return m_sAboutBoxInfo;}
        CString&        GetArcvPath()       { return m_sArcvPath;}
        CMainFrame&     GetFrame()          { return m_Frame;}
        UINT            GetMaxMRUSlots()    { return m_nMaxMRUSlots;}
        CMainFrame&     TheFrame()          { return m_Frame;}

    protected:
                void    Serialize(CArchive &ar);

    private:
          // static methods
        static  CString MakeAppDataPath(const CString& subpath);

          // private data
        CString         m_sArcvPath,  // archive file name
                        m_sAboutBoxInfo; // credits
        CMainFrame      m_Frame;      // the main frame object
        UINT            m_nMaxMRUSlots;
          // static data
        static ULONG    DatInt(LPCTSTR);
        static CString  IntDat(ULONG);
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

