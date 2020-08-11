/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|             Copyright (c) 2020, Robert C. Tausworthe, David Nash             |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the the CDoc class for this application
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

#ifndef SDI_DOC_H
#define SDI_DOC_H

/*============================================================================*/
    class
CDoc : public CObject                           /*
    Application document interface class
*-----------------------------------------------------------------------------*/
{
    public:

        CDoc();
        virtual ~CDoc(){}

        int     GetDocLength() const;
        CString GetDocOpenFileName(const CString&) const;
        const CString&
                GetDocPath() const      { return m_docPath; }
        CString GetDocRecord(int, int left = 0, int length = -1) const;
        CString GetDocSaveFileName(const CString&) const;
        int     GetDocWidth() const;
        CTime   GetTimeFromStr(LPCTSTR szTime, int nDST = -1 ) const;
        bool    OpenDoc(const CString &);
        void    NewDocument();
        void    PushContent(const CString&);
        bool    SaveDoc();
        bool    SaveDocAs();
        CString Truth(bool b)           {return b ? _T("true") : _T("false");}

      // public data members
        static const    CString m_compiledOn;  // date, mmm dd yyyy

    protected:
        virtual void    Serialize(CArchive& ar);  // moved from public

    private:
        CString     m_docPath;
        int         m_docWidth;             // length, in characters
        LPCTSTR     m_fileDlgFilter;        // file dialog filter
        std::vector<CString> m_docContent;  // array of document lines
};
/*----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
