/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CDoc class for the CommonDialogs 
    sample program using the Win32++ Windows interface classes. This class
    provides the document management functions for the application.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

class CMainFrame;
class CView;

/*============================================================================*/
    class 
CDoc    : public CObject                                                    /*

    This application's document management class.
*-----------------------------------------------------------------------------*/
{
    public:
        CDoc();
        virtual ~CDoc(){}

        CString GetDocDir()  const 
                    { CFile f; f.SetFilePath(m_docPath);
                      return f.GetFileDirectory();}
        CString GetDocPath() const { return m_docPath;}
        BOOL    IsDirty();
        BOOL    IsOpen() const {return m_isOpen;}
        BOOL    MakeNewDoc(LPCTSTR);
        void    OnCloseDoc();
        void    OnFindReplace(UINT, WPARAM, LPARAM);
        void    OnFRFindNext(MyFindReplaceDialog*);
        void    OnFRReplaceAll(MyFindReplaceDialog*);
        void    OnFRReplaceCurrent(MyFindReplaceDialog*);
        void    OnFRTerminating(MyFindReplaceDialog*);
        BOOL    OnSaveDoc();
        BOOL    OpenDoc(LPCTSTR);
        void    SetDirty(BOOL b);
        void    SetDataPath(CView*);

    private:
        CHARRANGE   FindNext(const MyFindReplaceDialog&, CHARRANGE);
        CRichEditView& GetRichView() const;
        void        NotFound(const MyFindReplaceDialog&);
        void        Serialize(CArchive &ar);
            
        BOOL        m_isOpen;       // the document status  
        CString     m_findNext;     // current string to find
        CString     m_replaceWith;  // replacement string
        CString     m_docPath;      // the path of the open document
        CView*      m_data;         // path to the document data
};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
