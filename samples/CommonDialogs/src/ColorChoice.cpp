/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]          (ColorChoice.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CColorChoice class for
    applications using the Win32++ Windows interface classes. This class
    extends the CColoDialog class to select colors for a number of items
    whose usage descriptors are displayed in a ClistBoxDlg. Instructions
    on the application of this class appears below.

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
    using its index in the GetTableColor(), GetBrush(), GetTableUsage(),
    SetTableColor(), and SetTableUsage() methods.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "ColorChoice.h"

/*============================================================================*/
    CColorChoice::
CColorChoice()                                                              /*

    Construct a color choice object with or without initial values.
*-----------------------------------------------------------------------------*/
    : m_selection(0)
{
    ClearColorTable();
    InitCustomColors();
    SetBoxTitle(_T("Color"));
    m_LBDlg.SetBoxTitle(_T("Choose color to change."));
}

/*============================================================================*/
    void CColorChoice::
        AddColorChoice(UINT id, LPCTSTR usage, COLORREF color)                      /*

            Add the (id, name, color) tuple to the color table, return TRUE on
            success, FALSE otherwise.
        *-----------------------------------------------------------------------------*/
{
    ctl_color triplet;
    triplet.id = id;
    triplet.usage = usage;
    triplet.color = color;
    m_colorTable.push_back(triplet);
}

/*============================================================================*/
    INT_PTR CColorChoice::
DoModal(HWND owner /* = 0 */)                                               /*

    Show the CListBoxDlg dialog box with the list of candidate control
    categories and, if a one is selected, show the CColorDialog box to
    select a color for that control category. If no category or color is
    selected, return IDCANCEL. Otherwise, return IDOK. On exit, the indes of
    the selected color can be accessed via GetSelectedColorID().
*-----------------------------------------------------------------------------*/
{
      // determine a common owner for the two dialog boxes
    if (owner == 0)
        owner = GetParameters().hwndOwner;
    if (owner == 0)
        owner = GetApp()->GetMainWnd();
      // prepare the list box dialog: load the color table choice items
    m_LBDlg.ClearList();
    for (UINT i = 0; i < static_cast<UINT>(m_colorTable.size()); i++)
        m_LBDlg.AddListItem(m_colorTable[i].usage);
      // Make the control be modal so the choice is returned at
      // termination.
    m_selection = static_cast<UINT>(-1);
    INT_PTR selection = m_LBDlg.DoModal(owner);
      // if invalid
    if (selection != INT_MAX)
    {     // register the current control color in the color choice struct
          // with the base class
        SetColor(m_colorTable[selection].color);
          // invoke the control and save the result on success
        if(CColorDialog::DoModal(owner) ==  IDOK)
        {
            m_colorTable[selection].color = CColorDialog::GetColor();
            m_selection = m_colorTable[selection].id;
        }
    }
    return (m_selection == INT_MAX ? IDCANCEL : IDOK);
}

/*============================================================================*/
    COLORREF CColorChoice::
GetTableColor(UINT id) const                                               /*

    Return the color found in the color table having the given id
    identifier.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(id);
    if (idx == static_cast<UINT>(-1))
        return RGB(0, 0, 0);

    return m_colorTable[idx].color;
}

/*============================================================================*/
    CBrush CColorChoice::
GetBrush(UINT id) const                                                    /*

    Return a solid brush with the color found in the color table at the
    given id identifier. If the table has not yet been populated, return
    a black brush. If id is not found, throw an exception.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(id);
    COLORREF color = (idx == static_cast<UINT>(-1) ?
        RGB(0, 0, 0) : m_colorTable[idx].color);
    CBrush br;
    br.CreateSolidBrush(color);
        return br;
}

/*============================================================================*/
    UINT CColorChoice::
GetTableIndex(UINT id) const                                               /*

    Return the color table index of the entry having the given id. Return
    (UINT)(-1) if id is not in the table.
*-----------------------------------------------------------------------------*/
{
      // ignore the invocation if the table is empty
    UINT n = static_cast<UINT>(m_colorTable.size());
    for (UINT idx = 0; idx < n; ++idx)
        if (m_colorTable[idx].id == id)
            return idx;

     return static_cast<UINT>(-1); // default value
}

/*============================================================================*/
    CString CColorChoice::
GetTableUsage(UINT id) const                                               /*

    Return the usage field of the ctl_color triplet corresponding having
    the given id.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(id);
    CString usage = (idx == static_cast<UINT>(-1) ? _T("") :
        m_colorTable[idx].usage.c_str());
    return  usage;
}

/*============================================================================*/
    void CColorChoice::
InitCustomColors()                                                          /*

    Initialize the custom color table entries to default values. Override
    this member to provide an application-dependent array.
*-----------------------------------------------------------------------------*/
{
      // initial values for custom colors in color dialog
    COLORREF* colors = GetCustomColors();
    colors[0]  = COLOR_LT_GRAY;
    colors[1]  = COLOR_GRAY;
    colors[2]  = COLOR_LT_BLUE;
    colors[3]  = COLOR_DK_BLUE;
    colors[4]  = COLOR_BROWN;
    colors[5]  = COLOR_DK_BROWN;
    colors[6]  = COLOR_LT_YELLOW;
    colors[7]  = COLOR_YELLOW;
    colors[8]  = COLOR_DK_YELLOW;
    colors[9]  = COLOR_LT_CYAN;
    colors[10] = COLOR_DK_CYAN;
    colors[11] = COLOR_LT_GREEN;
    colors[12] = COLOR_DK_GREEN;
    colors[13] = COLOR_LT_RED;
    colors[14] = COLOR_LT_MAGENTA;
    colors[15] = COLOR_DK_MAGENTA;
}

/*============================================================================*/
    BOOL CColorChoice::
OnInitDialog()                                                              /*

    Perform any actions required for initialization of this object when
    the color choice is being initialized.
*-----------------------------------------------------------------------------*/
{
      // do the base class initialization first (it currently does nothing)
    CDialog::OnInitDialog();
    SetWindowTitle();
    return TRUE;
}

/*============================================================================*/
        void CColorChoice::
Serialize(CArchive &ar)                                                     /*

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
        ar << static_cast<UINT>(m_colorTable.size());
        std::vector<ctl_color>::iterator it;
        for (it = m_colorTable.begin(); it < m_colorTable.end(); ++it)
        {
            ar << (*it).id;
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
        m_colorTable.resize(n);
        std::vector<ctl_color>::iterator it;
        for (it = m_colorTable.begin(); it < m_colorTable.begin() + n;
            ++it)
        {
            ar >> (*it).id;
            ar >> (*it).usage;
            ar >> (*it).color;
        }
    }

    CColorDialog::Serialize(ar);
}

/*============================================================================*/
    void CColorChoice::
SetTableColor(UINT id, COLORREF rgb)                                       /*

    Set the color entry of the color table entry with the given id to
    the specified rgb color.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(id);
    if (idx == static_cast<UINT>(-1))
        return;

    m_colorTable[idx].color = rgb;
}

/*============================================================================*/
    void CColorChoice::
SetTableUsage(UINT id, LPCTSTR s)                                          /*

    Set the usage entry of the color table entry with the given id to
    the specified s string.
*-----------------------------------------------------------------------------*/
{
    UINT idx = GetTableIndex(id);
    if (idx == static_cast<UINT>(-1))
        return;

    m_colorTable[idx].usage = s;
}
/*----------------------------------------------------------------------------*/
