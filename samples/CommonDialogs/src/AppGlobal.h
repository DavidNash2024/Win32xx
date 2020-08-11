/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (AppGlobal.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CAppGlobal class, which is
    used by the CApp class in an App/Frame/Doc/View architecture for
    providing a set of commonly needed items in applications using the
    Win32++ Windows interface classes, Copyright (c) 2005-2015 David Nash,
    under permissions granted therein.

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

    Declaration of the CAppGlobal class

*******************************************************************************/

#ifndef APP_CPROLOG_H
#define APP_CPROLOG_H

class CAppGlobal
{
    public:
        CAppGlobal();
        ~CAppGlobal();

        const CString& GetAboutBoxInfo() const { return m_sAboutBoxInfo;}
        const CString& GetAppDir() const       { return m_sAppDir;}
        const CString& GetAppName() const      { return m_sAppName;}
        const CString& GetAppPath() const      { return m_sAppPath;}
        const CString& GetAppTitle() const     { return m_sAppTitle;}
        const CString& GetArcvDir() const      { return m_sArcvDir;}
        const CString& GetArcvPath() const     { return m_sArcvPath;}
        const CString& GetCompiler() const     { return m_sCompiler;}
        const CString& GetCompileDate() const  { return m_sCompiled_on;}
        const CString& GetDocExt() const       { return m_sDocExt;}
        const CString& GetFileFilter() const   { return m_sFileFilter;}
        UINT           GetMaxMRU() const       { return m_nMaxMRUSlots;}
        const CString& GetWin32Vers() const    { return m_sWin32Version;}
        CString MakeAppDataPath(const CString& subpath) const;


    protected:

    private:
          // private data
        UINT    m_nMaxMRUSlots;  // maximum allowed MRU entries
        CString m_sAboutBoxInfo; // About box information
        CString m_sAppTitle;     // name of app, no directory, with ext
        CString m_sAppName;      // name of app: no directory, no ext
        CString m_sAppDir;       // directory of this app
        CString m_sAppPath;      // path to this app
        CString m_sArcvDir;      // directory holding the archive file
        CString m_sArcvPath;     // archive file name
        CString m_sCompiler;     // compiler designation
        CString m_sCompiled_on;  // compilation date
        CString m_sDocExt;       // document default extension
        CString m_sFileFilter;   // do file dialog filter
        CString m_sWin32Version; // version of Win32 framework used
};
/*----------------------------------------------------------------------------*/
#endif  // CAPP_PROLOG_H
