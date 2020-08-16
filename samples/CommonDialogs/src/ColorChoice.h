/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (ColorChoice.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CColorChoice popup dialog class 
    for this application using the Win32++ Windows interface classes. 
    Information on the use of this class appears in the CColorChoice.cpp file.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CCOLORCHOICE_H
#define CCOLORCHOICE_H

#include "ColorDefs.h"
#include "ListBoxDlg.h"

  // id-usage-color triples forming the m_colorTable array
struct ctl_color {UINT id; CString  usage;  COLORREF color;};

/******************************************************************************/
    class
CColorChoice   : public CColorDialog                                        /*

*-----------------------------------------------------------------------------*/
{
    public:
        CColorChoice();
        virtual ~CColorChoice(){}
        
        virtual void    AddColorChoice(UINT, LPCTSTR, COLORREF);
        virtual void    ClearColorTable(){m_colorTable.clear();}
        virtual INT_PTR DoModal(HWND owner = 0);
        virtual CBrush  GetBrush(UINT) const; 
        virtual CListBoxDlg& GetListBoxDlg() {return m_LBDlg;}
        virtual UINT    GetSelectedColorID() const {return m_selection;}
        virtual COLORREF GetTableColor(UINT id) const;
        virtual UINT    GetTableIndex(UINT id) const;
        virtual CString GetTableUsage(UINT id) const; 
        virtual void    SetBoxTitle(LPCTSTR title) {m_boxTitle = title;}
        virtual void    SetTableColor(UINT id, COLORREF rgb);
        virtual void    SetTableUsage(UINT id, LPCTSTR s);

    protected:
        virtual BOOL    OnInitDialog(); 
        virtual void    InitCustomColors();        
        virtual void    SetWindowTitle() const {SetWindowText(m_boxTitle);}   
        virtual void    Serialize(CArchive &ar);

    private:
        CListBoxDlg m_LBDlg;                 // the list box dialog
        CString     m_boxTitle;              // the color dialog box title
        UINT        m_selection;             // the selected color index
        std::vector<ctl_color> m_colorTable; // usage-color pairs
};
/*----------------------------------------------------------------------------*/
#endif  // CCOLORCHOICE_H
