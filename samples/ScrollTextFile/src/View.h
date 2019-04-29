/* (14-Nov-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CScrollWnd demo application view 
    class for a typical Windows application using the Win32++ Windows interface 
    classes, Copyright (c) 2005-2017 David Nash, under permissions granted 
    therein.

    Caveats: These materials are available under the same provisions as found 
    in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CAPP_VIEW_H
#define CAPP_VIEW_H

#include "FontEx.h"

/*============================================================================*/
    class
CColorExDialog : public CColorDialog                                      /*

    Extend the CColorDialog class to change the window title.
*-----------------------------------------------------------------------------*/
{
    public:
        CColorExDialog(COLORREF clrInit = 0, DWORD dwFlags = 0)
            : CColorDialog(clrInit, dwFlags | CC_ENABLEHOOK |
            CC_RGBINIT | CC_ANYCOLOR)
        {
            SetBoxTitle(_T("Color..."));
        }

        ~CColorExDialog(){}

        void    SetBoxTitle(const CString& title) { m_sBoxTitle = title;}

    protected:
        virtual BOOL    OnInitDialog()
                        {
                            CColorDialog::OnInitDialog();
                            SetWindowText(m_sBoxTitle);
                            return TRUE;
                        }

    private:
        CString     m_sBoxTitle;
};


/*============================================================================*/
    class
CView : public CScrollWnd                                                   /*

    The application scrolling window class.
*-----------------------------------------------------------------------------*/
{
    public:
        CView();
        virtual ~CView();

        virtual CPoint  DevToScrl(CPoint) const;
        virtual void    ClientFontChoice();
        virtual void    OnColorChoice();
        virtual CPoint  ScrlToDev(CPoint) const;
        virtual void    Serialize(CArchive &ar);
        virtual void    SetNewAppSize();

    protected:
        virtual void    InitCustomColors();     
        virtual void    OnInitialUpdate();
        virtual void    Paint(CDC& dcMem);
        virtual void    PreCreate(CREATESTRUCT &cs);
        virtual void    PreRegisterClass(WNDCLASS &wc);
        virtual BOOL    PreTranslateMessage(MSG &Msg);
        virtual void    SetScrollAppInfo();
        virtual void    TextLineOut(CDC&, UINT, UINT, const CStringW&);
        virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

    private:
          // private data members
        CFontEx     m_VuFont;               // the view display font
        COLORREF    m_rgbCustomColors[16];  // custom colors defined
        CSize       m_doc_extent;           // the document extent
};
/*----------------------------------------------------------------------------*/
#endif  //CAPP_VIEW_H
