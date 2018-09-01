/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the  CView class for the
    CommonDialogs sample SDI application using the Win32++ Windows interface
    classes, Copyright (c) 2005-2016 David Nash, under permissions granted
    therein.

    This particular view class demonstrates methods for selection of client
    background color, selection of edit box font, and use of external
    serialization files.

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

    Declaration of the CView class

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

class CPrintRichView;

/*******************************************************************************

    Local constants and types                                   */

   // Mnemonic identifiers for entries in the m_ColorTable array.  The nID
   // field should be one of these identifiers for clarity. Default color is 0
enum CtlColors
{
    DfltClr = 0,
    DlgBg,                   // dialog
    REdTxFg, REdTxBg, REdBg, // rich edit
    SBBg,                    // status bar
    EndColors                // end color IDs
};

/*============================================================================*/
    class 
CView : public CDialog                          /*

    This application's View class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
    public:
        CView(UINT nResID);
        virtual ~CView(){}
        
        virtual void    AttachControl(UINT nIDC, CWnd& rCtl);
        virtual HWND    Create(HWND hParent);
                CFont&  GetEditFont() { return m_EditFont;}
         CRichEditView& GetREView() { return m_RichEdit;}
                void    NoDocOpen();
                BOOL    OnDropFiles(HDROP hDroinfo);
        virtual void    OnPrintDocument();
        virtual void    OnPrintPreview();
                void    Register(CDoc*);
                void    SetEditFont(const CFont& f);
                void    SetRichEditColors(COLORREF, COLORREF, COLORREF);

          // public data members

    protected:
        virtual BOOL    AddToolTip(HWND, UINT nID);
        virtual BOOL    AddToolTip(HWND, UINT nID, const CString & s);
        virtual void    AssignToolTips();
        virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
                CDoc&   GetDoc() {return *m_pDoc;}
        virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
        virtual BOOL    OnInitDialog();
        virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
        virtual void    OnOK();
        virtual void    Serialize(CArchive &ar);

    private:
          // private data members
        HWND            m_hParent;      // handle of parent frame
        CToolTip        m_ToolTip;      // form tool tips
        CFont           m_EditFont;     // edit box font
          // controls on the view
        CRichEditView   m_RichEdit;     // the view of the document
        CDoc*           m_pDoc;         // the view's document
};
/*----------------------------------------------------------------------------*/
#endif // SDI_VIEW_H
