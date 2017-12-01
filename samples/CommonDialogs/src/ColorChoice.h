/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (ColorChoice.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of a generic CColorChoice popup
    dialog class for applications using the Win32++ Windows interface
    classes, Copyright (c) 2005-2016 David Nash, under permissions granted
    therein. Information on the use of this class appears in the
    CColorChoice.cpp file.

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

    Declaration of the CColorChoice class

*******************************************************************************/

#ifndef CCOLORCHOICE_H
#define CCOLORCHOICE_H

#include "ColorDefs.h"
#include "ListBoxDlg.h"
#include "ListBoxDlgRC.h"

/*******************************************************************************

    Local struct and enum definitions                               */

  // id-usage-color triples forming the m_ColorTable array
struct ctl_color {UINT nID; CString  usage;  COLORREF color;};

/******************************************************************************/
    class
CColorChoice   : public CColorDialog                    /*

*-----------------------------------------------------------------------------*/
{
    public:
        CColorChoice();
        virtual ~CColorChoice(){}
        
        virtual BOOL    AddColorChoice(UINT, const CString&, COLORREF);
            void    ClearColorTable(){m_ColorTable.clear();}
            UINT    DeleteTableEntry(UINT index);
        virtual INT_PTR DoModal(HWND hWndOwner = 0);
            CBrush  GetBrush(UINT nID);
        virtual CListBoxDlg& GetListBoxDlg() {return m_LBDlg;}
            UINT    GetSelectedColorID() {return m_nSelection;}
            COLORREF GetTableColor(UINT nID);
            UINT    GetTableIndex(UINT nID);
            UINT    GetTableSize() { return m_ColorTable.size();}
            CString GetTableUsage(UINT nID); 
            void    SetBoxTitle(const CString& title) {m_sBoxTitle = title;}
        virtual void    SetTableColor(UINT nID, COLORREF rgb);
        virtual void    SetTableUsage(UINT nID, const CString& s);

    protected:

        virtual BOOL    OnInitDialog(); 
        virtual void    SetWindowTitle() const
                    {SetWindowText(m_sBoxTitle);}   
        virtual void    Serialize(CArchive &ar);

    private:
        void InitCustomColors();        

          // private data
        CListBoxDlg     m_LBDlg;    // the list box dialog
        CString     m_sBoxTitle;    // the color dialog box title
        UINT            m_nSelection;   // the selected color index
        std::vector<ctl_color> m_ColorTable;   // usage-color pairs
};
/*----------------------------------------------------------------------------*/
#endif  // CCOLORCHOICE_H
