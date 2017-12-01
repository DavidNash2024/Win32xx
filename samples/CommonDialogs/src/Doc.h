/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CDoc class for the
    CommonDialogs sample  application using the Win32++ Windows interface
    classes, Copyright (c) 2005-2016 David Nash, under permissions granted
    therein.

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

    Declaration of the CDoc class

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

class CMainFrame;
class CView;

/*============================================================================*/
    class 
CDoc    : public CObject                        /*

    This application's document class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
    public:

        CDoc();
        virtual ~CDoc();
        virtual const CString&  GetExt();
        virtual const CString&  GetFilter();

        const  CString& GetFilePath()
                    { return (m_open_doc_path.IsEmpty() ?
                         m_Doc_file.GetFilePath() :
                     m_open_doc_path);}
        virtual BOOL    IsDirty();
        virtual BOOL    IsOpen() {return m_Doc_is_open;}
        virtual BOOL    MakeNewDoc(const CString&);
        virtual void    OnCloseDoc();
        virtual void    OnCopy();
        virtual void    OnCut();
        virtual void    OnDelete();
        virtual void    OnPaste();
        virtual void    OnFindReplace(UINT, WPARAM, LPARAM);
        virtual void    OnFRFindNext(MyFindReplaceDialog*);
        virtual void    OnFRReplaceAll(MyFindReplaceDialog*);
        virtual void    OnFRReplaceCurrent(MyFindReplaceDialog*);
        virtual void    OnFRTerminating(MyFindReplaceDialog*);
        virtual void    OnNewDoc();
        virtual void    OnOpenDoc();
        virtual void    OnRedo();
        virtual void    OnUndo();
        virtual BOOL    OnSaveDoc();
        virtual void    OnSaveDocAs();
        virtual void    OnPageSetup();
        virtual BOOL    OpenDoc(const CString &);
            void    Register(CMainFrame*, CView*);
            void    SetDirty(BOOL b);
        virtual void    SetExt(const CString& ext)
                    { m_Doc_file_ext = ext;}
        virtual void    SetFilter(const CString &s)
                    { m_Doc_file_filter = s;}

          // public data members

    protected:
            CMainFrame& GetFrame() { return *m_pParent;}
         CRichEditView& GetREView();
                CView&  GetView() { return *m_pView;}
        virtual void    Serialize(CArchive &ar);
        
            BOOL    m_UnicodeMode,  // compiled in Unicode mode
                m_UnicodeFile;  // probably Unicode if TRUE

    private:
              CHARRANGE FindNext(const MyFindReplaceDialog&, CHARRANGE);
            void    NotFound(const MyFindReplaceDialog&);
            
          // private data
        CFile   m_Doc_file;        // holds the document name, path, etc.
        BOOL    m_Doc_is_open;     // the document status
            CString m_Doc_file_ext,    // default file extension
                m_Doc_file_filter, // file dialog filter
                m_find_next,       // current string to find
                m_replace_with,    // replacement string
                m_open_doc_path;   // the path of the open document
        CView*  m_pView;           // the view for this document
        CMainFrame* m_pParent;     // the parent frame
};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
