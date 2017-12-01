/* [Tab/Indent: 8/8][Line/Box: 80/74]                      (CustomStatusBar.h) *
********************************************************************************

    Contents Description: Declaration of the CustomStatusBar class for a 
    sample program demonstrating an expanded use of the status bar within
    the Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

    This particular program was adapted from the StatusBar sample progam
    in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
    definition of the status bar makeup by way of a data structure, in a
    similar fashion as that appearing in the Microsoft Foundation Classes 
    (MFC), and to override the Win32++ normal statusbar definition of four 
    parts to any number suitable for an application.

    The code changes to the StatusBar sample program only occur in the 
    CMainFrame and MyStatusBar classes. However, the entire program has
    been reformatted for better readability, per the Programming Notes,
    below.

    Usage of the class is discussed in the CustomStatusBar.cpp file
    preamble.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.

    Programming Notes:
                The programming conventions used here roughly follow those 
        established by the 1997-1999 Jet Propulsion Laboratory Deep 
        Space Network Planning and Preparation Subsystem project for 
        C++ programming.

*******************************************************************************/

#ifndef CUSTOMSTATUSBAR_H
#define CUSTOMSTATUSBAR_H

#include "Hyperlink.h"

const int SB_AUTOSIZE = -1;

const COLORREF  SBEraseColor[2] = {RGB(125, 230, 255), RGB(250, 150, 150)},
        SBBkgrColor =  RGB(125, 230, 255),
        ODTextColor =  RGB(10,20,250),
        ODBkgrColor[2]  = {RGB(230, 180,   0), RGB(240, 210,  90)};

/*============================================================================*/
    struct 
PartInfo                                /*

    The PartInfo struct contains the numeric identifier, part size, and 
    an optional string value of a status bar part. 
*-----------------------------------------------------------------------------*/
{
    PartInfo(UINT nID, int sz, UINT Style = 0)
        {ID = nID, width = sz; style = Style;}
    UINT    ID;
    int width;
    UINT    style;
    CString old;
};

/*******************************************************************************

    Declaration of the CustomStatusBar class.

*============================================================================*/
    class 
CustomStatusBar : public CStatusBar                 /*

*-----------------------------------------------------------------------------*/
{
    public:
        CustomStatusBar();
        virtual ~CustomStatusBar() {}

        virtual void    AddPartInfo(UINT nID, int size, UINT style = 0)
                    {   PartInfo pi(nID, size, style);
                    m_statusbar_part.push_back(pi);}
        virtual void    PositionCustomParts();
            void    SetOwnerDrawText(LPCTSTR odmsg)
                    { m_ODMessage = odmsg;
                      SetStatusIndicators();}
        virtual void    SetStatusIndicators();

    protected:
            int IDtoPart(UINT);
        virtual void    OnAttach();
        virtual LRESULT OnMessageReflect(UINT, WPARAM, LPARAM);
        virtual BOOL    OnEraseBkgnd(CDC& dc);
        virtual void    PreCreate(CREATESTRUCT& cs);
        virtual LRESULT WndProc(UINT, WPARAM, LPARAM);


    private:
        CHyperlink  m_Hyperlink;
        CProgressBar    m_ProgressBar;
        CString     m_ODMessage;
        std::vector<PartInfo> m_statusbar_part;
};
/*----------------------------------------------------------------------------*/
#endif // CUSTOMSTATUSBAR_H
