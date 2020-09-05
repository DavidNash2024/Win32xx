/* [Tab/Indent: 8/8][Line/Box: 80/74]                      (CustomStatusBar.h) *
********************************************************************************

    Contents Description: Declaration of the CustomStatusBar class for a
    sample program demonstrating an expanded use of the status bar within
    the Win32++ Windows framework.

    Usage of the class is discussed in the CustomStatusBar.cpp file
    preamble.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CUSTOMSTATUSBAR_H
#define CUSTOMSTATUSBAR_H

#include "Hyperlink.h"

const int SB_AUTOSIZE = -1;

const COLORREF
        SBEraseColor[2] = {RGB(125, 230, 255), RGB(250, 150, 150)},
        SBBkgrColor     =  RGB(125, 230, 255),
        ODTextColor     =  RGB(10, 20, 250),
        ODBkgrColor[2]  = {RGB(230, 180,   0), RGB(240, 210,  90)};

/*============================================================================*/
    struct
PartInfo                                                                    /*

    The PartInfo struct describes the makeup of a statusbar part, namely its
    numeric identifier, size, style, and current string contents.
*-----------------------------------------------------------------------------*/
{
    PartInfo(UINT id, int sz, UINT Style = 0)
        {ID = id, width = sz; style = Style;}

    UINT    ID;
    int     width;
    UINT    style;
    CString old;
};

/*******************************************************************************

    Declaration of the CustomStatusBar class.

*============================================================================*/
    class
CustomStatusBar : public CStatusBar                                         /*

*-----------------------------------------------------------------------------*/
{
    public:
        CustomStatusBar();
        virtual ~CustomStatusBar() {}

        virtual void    AddPartInfo(UINT id, int size, UINT style = 0);
        virtual void    PositionCustomParts();
                void    SetOwnerDrawText(LPCTSTR odmsg);
        virtual void    SetStatusIndicators();

    protected:
                int     IDtoPart(UINT);
        virtual void    OnAttach();
        virtual LRESULT OnMessageReflect(UINT, WPARAM, LPARAM);
        virtual BOOL    OnEraseBkgnd(CDC& dc);
        virtual void    PreCreate(CREATESTRUCT& cs);
        virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

    private:
        CHyperlink      m_hyperlink;
        CProgressBar    m_progressBar;
        CString         m_ownerDrawMsg;
        std::vector<PartInfo> m_statusBarParts;
};
/*----------------------------------------------------------------------------*/
#endif // CUSTOMSTATUSBAR_H
