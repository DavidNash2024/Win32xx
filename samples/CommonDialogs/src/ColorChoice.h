/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (ColorChoice.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CColorChoice popup dialog class
    for this application using the Win32++ Windows interface classes.
    Information on the use of this class appears in the CColorChoice.cpp file.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CCOLORCHOICE_H
#define CCOLORCHOICE_H

#include "ColorDefs.h"
#include "ListBoxDlg.h"

 // id-usage-color triples forming the m_colorTable array
struct ctl_color
{
    ctl_color() : id(0), color(0) {}
    UINT id;
    CString  usage;
    COLORREF color;
};

/******************************************************************************/
    class
CColorChoice   : public CColorDialog                                        /*

*-----------------------------------------------------------------------------*/
{
    public:
        CColorChoice();
        virtual ~CColorChoice(){}

        void    AddColorChoice(UINT, LPCTSTR, COLORREF);
        void    ClearColorTable(){m_colorTable.clear();}
        void    InitCustomColors();
        INT_PTR DoModal(HWND owner = 0);
        CBrush  GetBrush(UINT) const;
        CListBoxDlg& GetListBoxDlg() {return m_LBDlg;}
        UINT    GetSelectedColorID() const {return m_selection;}
        COLORREF GetTableColor(UINT id) const;
        UINT    GetTableIndex(UINT id) const;
        CString GetTableUsage(UINT id) const;
        void    SetBoxTitle(LPCTSTR title) {m_boxTitle = title;}
        void    SetTableColor(UINT id, COLORREF rgb);
        void    SetTableUsage(UINT id, LPCTSTR s);

    protected:
        virtual BOOL    OnInitDialog();
        virtual void    SetWindowTitle() const {SetWindowText(m_boxTitle);}
        virtual void    Serialize(CArchive &ar);

    private:
        CColorChoice(const CColorChoice&);                // Disable copy construction
        CColorChoice& operator = (const CColorChoice&);   // Disable assignment operator

        CListBoxDlg m_LBDlg;                 // the list box dialog
        CString     m_boxTitle;              // the color dialog box title
        UINT        m_selection;             // the selected color index
        std::vector<ctl_color> m_colorTable; // usage-color pairs
};
/*----------------------------------------------------------------------------*/
#endif  // CCOLORCHOICE_H
