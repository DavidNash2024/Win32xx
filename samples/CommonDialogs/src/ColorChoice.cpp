/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]          (ColorChoice.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CColorChoice class for
    applications using the Win32++ Windows interface classes, Copyright
    (c) 2005-2016 David Nash, under permissions granted therein. This class
    extends the CColoDialog class to select colors for a number of items
    whose usage descriptors are displayed in a ClistBoxDlg. Instructions
    on the application of this class appears below.

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

    Usage Information: This class populates a table of usage-color pairs
    that may be used by an application or a derived class.  The class then
    utilizes the CListBoxDlg class to display the usage name strings in
    a list box and the CColorDialog class (from which this class derives)
    to assign colors to a selected usages displayed in the list box. The
    user populates the color by table using the ClearColorTable() to
    empty any current content and then calling the AddColorChoice() method
    to add usage-color structs to the color table.  The usage names appear
    in the list box for selection when the DoModal() member is engaged.

    The user calls upon the DoModal() method to display the color selection
    ListBox and thence the color choice dialog to supply the color for that
    selection.  Access to a particular element in the color table is made
    using its index in the DeleteTableEntry(), GetTableColor(), GetBrush(),
    GetTableUsage(), SetTableColor(), and SetTableUsage() methods.

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Implementation of the CColorChoice class

*******************************************************************************/

#include "stdafx.h"
#include "ColorChoice.h"
#include "ListBoxDlgRC.h"

/*============================================================================*/
    CColorChoice::
CColorChoice()                              /*

    Construct a color choice object with or without initial values.
*-----------------------------------------------------------------------------*/
{
    ClearColorTable();
    SetBoxTitle(_T("Color"));
    m_LBDlg.SetBoxTitle(_T("Choose color to change."));
}

/*============================================================================*/
    BOOL CColorChoice::
AddColorChoice(UINT nID, const CString& usage, COLORREF color)      /*

    Add the (nID, name, color) tuple to the color table, return TRUE on
    success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    UINT size = GetTableSize();
    ctl_color triple;
    triple.nID = nID;
    triple.usage = usage;
    triple.color = color;
    m_ColorTable.push_back(triple);
    return (GetTableSize() == size + 1);
}

/*============================================================================*/
    UINT CColorChoice::
DeleteTableEntry(UINT index)                        /*

    Description and explanation of parameters
*-----------------------------------------------------------------------------*/
{
    return (m_ColorTable.erase(m_ColorTable.begin() + index) -
         m_ColorTable.begin());
}

/*============================================================================*/
    INT_PTR CColorChoice::
DoModal(HWND hWndOwner /* = 0 */)                   /*

        Show the CListBoxDlg dialog box with the list of candidate control
    categories and, if a one is selected, show the CColorDialog box to
    select a color for that control category. If no category or color is
    selected, return IDCANCEL. Otherwise, return IDOK. On exit, the indes of
    the selected color can be accessed via GetSelectedColorID().
*-----------------------------------------------------------------------------*/
{
      // determine a common window for the two dialog boxes
    if (hWndOwner == 0)
        hWndOwner = GetParameters().hwndOwner;
    if (hWndOwner == 0)
        hWndOwner = GetApp().GetMainWnd();
      // prepare the list box dialog: load the color table choice items
    m_LBDlg.ClearList();
    for (UINT i = 0; i < GetTableSize(); i++)
        m_LBDlg.AddListItem(m_ColorTable[i].usage);
      // Make the control be modal so the choice is returned at
      // termination.
    m_nSelection = static_cast<UINT>(-1);
    int selection = m_LBDlg.DoModal(hWndOwner);
      // if invalid, go no further
    if (selection < 0)
        return IDCANCEL;

      // register the current control color in the color choice struct
      // with the base class
    SetColor(m_ColorTable[selection].color);
      // invoke the control and save the result on success
    if(CColorDialog::DoModal(hWndOwner) ==  IDOK)
    {
        m_ColorTable[selection].color = CColorDialog::GetColor();
        m_nSelection =m_ColorTable[selection].nID;
        return IDOK;
    }
    return IDCANCEL;
}

/*============================================================================*/
    COLORREF CColorChoice::
GetTableColor(UINT nID)                         /*

    Return the color found in the color table having the given nID
    identifier.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(nID);
    if (idx == static_cast<UINT>(-1))
        return RGB(0, 0, 0);

    return m_ColorTable[idx].color;
}

/*============================================================================*/
    CBrush CColorChoice::
GetBrush(UINT nID)                          /*

    Return a solid brush with the color found in the color table at the
    given nID identifier. If the table has not yet been populated, return
    a black brush. If nID is not found, throw an exception.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(nID);
    COLORREF color = (idx == static_cast<UINT>(-1) ?
        RGB(0, 0, 0) : m_ColorTable[idx].color);
    CBrush br;
    br.CreateSolidBrush(color);
        return br;
}

/*============================================================================*/
    UINT CColorChoice::
GetTableIndex(UINT nID)                                                 /*

    Return the color table index of the entry having the given nID. Return
    static_cast<UINT>(-1) if nID is zero or the table is empty.  Throw an
    exception if nID is nonzero and is not in the table.
*-----------------------------------------------------------------------------*/
{
      // ignore the invocation if the table is empty
    if (GetTableSize() == 0 || nID == 0)
        return static_cast<UINT>(-1); // default value

    UINT idx = 0;
    std::vector<ctl_color>::iterator it;
    for (it = m_ColorTable.begin(); it != m_ColorTable.end() &&
        (*it).nID != nID; ++it, ++idx)
        ;
    if (idx >= GetTableSize())
        idx = static_cast<UINT>(-1);

    return idx;
}

/*============================================================================*/
    CString CColorChoice::
GetTableUsage(UINT nID)                                                 /*

    Return the usage field of the ctl_color triplet corresponding having
    the given nID.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(nID);
    CString usage = (idx == static_cast<UINT>(-1) ? _T("") : m_ColorTable[idx].usage.c_str());
    return  usage;
}

/*============================================================================*/
    void CColorChoice::
InitCustomColors()                                                      /*

    Initialize the custom color table entries to default values. Override
    this member to provide an application-dependent array.
*-----------------------------------------------------------------------------*/
{
    // initial values for custom colors in color dialog
    COLORREF rgbCustomColors[16];
    rgbCustomColors[0]  = RGB(0,     5,   5);
    rgbCustomColors[1]  = RGB(0,    15,  55);
    rgbCustomColors[2]  = RGB(0,    25, 155);
    rgbCustomColors[3]  = RGB(0,    35, 255);
    rgbCustomColors[4]  = RGB(10,    0,   5);
    rgbCustomColors[5]  = RGB(10,   20,  55);
    rgbCustomColors[6]  = RGB(10,   40, 155);
    rgbCustomColors[7]  = RGB(10,   60, 255);
    rgbCustomColors[8]  = RGB(100,   5,   5);
    rgbCustomColors[9]  = RGB(100,  25,  55);
    rgbCustomColors[10] = RGB(100,  50, 155);
    rgbCustomColors[11] = RGB(100, 125, 255);
    rgbCustomColors[12] = RGB(200, 120,   5);
    rgbCustomColors[13] = RGB(200, 150,  55);
    rgbCustomColors[14] = RGB(200, 200, 155);
    rgbCustomColors[15] = RGB(200, 250, 255);

    SetCustomColors(rgbCustomColors);
}

/*============================================================================*/
    BOOL CColorChoice::
OnInitDialog()                                                          /*

    Perform any actions required for initialization of this object when
    the color choice is being initialized.
*-----------------------------------------------------------------------------*/
{
      // do the base class initialization first (it currently does nothing)
    CDialog::OnInitDialog();
    SetWindowTitle();
    InitCustomColors();
    return TRUE;
}

/*============================================================================*/
        void CColorChoice::
Serialize(CArchive &ar)                                                 /*

        Called to serialize the current color choice table to or from the
    archive ar, depending on the sense of IsStoring(). Leaves the archive
    open for for further operations.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    if (ar.IsStoring())
    {
          // save the current color
        ar << GetParameters().rgbResult;
              // save the custom colors
        ArchiveObject ao(GetCustomColors(), 16 * sizeof(COLORREF) );
        ar << ao;
          // save the color table entries
        ar << GetTableSize();
        std::vector<ctl_color>::iterator it;
        for (it = m_ColorTable.begin(); it < m_ColorTable.end(); ++it)
        {
            ar << (*it).nID;
            ar << (*it).usage;
            ar << (*it).color;
        }
    }
    else    // recovering
    {
          // recover current color
        COLORREF rgbResult;
        ar >> rgbResult;
        SetColor(rgbResult); // set base class current color
              // read in the custom colors
        COLORREF cr[16];
        ArchiveObject ao(cr, 16 * sizeof(COLORREF));
        ar >> ao;
        SetCustomColors(cr);
          // recover the color table entries
        UINT n;
        ar >> n;
        m_ColorTable.resize(n);
        std::vector<ctl_color>::iterator it;
        for (it = m_ColorTable.begin(); it < m_ColorTable.begin() + n;
            ++it)
        {
            ar >> (*it).nID;
            ar >> (*it).usage;
            ar >> (*it).color;
        }
    }

    CColorDialog::Serialize(ar);
}

/*============================================================================*/
    void CColorChoice::
SetTableColor(UINT nID, COLORREF rgb)                                   /*

    Set the color entry of the color table entry with the given nID to
    the specified rgb color.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(nID);
    if (idx == static_cast<UINT>(-1))
        return;

    m_ColorTable[idx].color = rgb;
}

/*============================================================================*/
    void CColorChoice::
SetTableUsage(UINT nID, const CString& s)                               /*

    Set the usage entry of the color table entry with the given nID to
    the specified s string.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(nID);
    if (idx == static_cast<UINT>(-1))
        return;

    m_ColorTable[idx].usage = s;
}

