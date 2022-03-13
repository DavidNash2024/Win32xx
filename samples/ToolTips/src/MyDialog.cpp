/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"

//////////////////////////////////
// CMyDialog function definitions.
//
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
    m_info = (HICON)GetApp()->LoadIcon(IDI_INFO);
}

CMyDialog::~CMyDialog()
{
    DestroyIcon(m_info);
}

void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDC_BUTTON1:   return OnButton();
    case IDC_CHECK1:    return OnCheck1();
    case IDC_CHECK2:    return OnCheck2();
    case IDC_CHECK3:    return OnCheck3();

    case IDC_RADIO1:    // intentionally blank
    case IDC_RADIO2:
    case IDC_RADIO3:    return OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id);
    }

    return FALSE;
}

BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the edit control to m_Edit
    AttachItem(IDC_EDIT1, m_edit);

    // Attach the rich edit control to m_RichEdit
    AttachItem(IDC_RICHEDIT1, m_richEdit);

    // Put some text in the edit boxes
    SetDlgItemText(IDC_EDIT1, _T("Edit Control"));
    SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

    // Put some text in the list box
    for (int i = 0 ; i < 8 ; i++)
        SendDlgItemMessage(IDC_LIST1, LB_ADDSTRING, 0, (LPARAM) _T("List Box"));

    // Select the first radio button
    CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

    // Create the bubble tooltip
    m_bubbleTT.Create(*this);

    // Set the background colour
    m_bubbleTT.SetWindowTheme(L" ", L" ");  // Turn XP themes off
    m_bubbleTT.SetTipBkColor(RGB(150, 255, 255));

    // Add controls to the bubble tooltip
    m_bubbleTT.AddTool(GetDlgItem(IDC_RADIO1), _T("Radio Button 1"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_RADIO2), _T("Radio Button 2"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_RADIO3), _T("Radio Button 3"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_BUTTON1), _T("Button 1"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_CHECK1), _T("Check Box 1"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_CHECK2), _T("Check Box 2"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_CHECK3), _T("Check Box 3"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_LIST1), _T("List Box"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_STATIC1), _T("Picture in a static control"));
    m_bubbleTT.AddTool(GetDlgItem(IDC_STATIC3), _T("Status display"));
    m_bubbleTT.AddTool(GetDlgItem(IDOK), _T("OK Button"));

    // Modify tooltip for IDC_RADIO3
    TOOLINFO ti1 = m_bubbleTT.GetToolInfo(GetDlgItem(IDC_RADIO3));
    ti1.uFlags |= TTF_CENTERTIP;
    ti1.lpszText = const_cast<LPTSTR>(_T("Modified tooltip for Radio Button 3"));
    m_bubbleTT.SetToolInfo(ti1);

    // Create a standard tooltip for the Edit and RichEdit controls
    m_toolTip.Create(*this);

    // Set the background color
    m_toolTip.SetWindowTheme(L" ", L" ");    // Turn XP themes off
    m_toolTip.SetTipBkColor(RGB(255, 255, 125));

    // Add controls to the standard Tooltip.
    // The tooltip will request the text to display via a TTN_GETDISPINFO notification
    m_toolTip.AddTool(m_edit, LPSTR_TEXTCALLBACK);
    m_toolTip.AddTool(m_richEdit, LPSTR_TEXTCALLBACK);
    m_toolTip.SetMaxTipWidth(500);


#ifdef  TTM_SETTITLE    // not supported by some GNU compilers
    // Add Title and Icon to the tooltip (a pretty icon for Vista and above)
    if (GetWinVersion() >= 2600)
        m_toolTip.SetTitle((UINT)(UINT_PTR)(m_info), _T("Displaying the contents of the control ..."));
    else
        m_toolTip.SetTitle(TTI_INFO, _T("Displaying the contents of the control ..."));
#endif

    // Calculate left half and right have rectangles
    CRect leftRect = GetClientRect();
    leftRect.right = leftRect.right / 2;
    CRect rightRect = GetClientRect();
    rightRect.left = rightRect.right / 2;

    // Specify a tooltip using a RECT and a user ID
    m_bubbleTT.AddTool(*this, leftRect,  1, _T("Client area, left side"));
    m_bubbleTT.AddTool(*this, rightRect, 2, _T("Client area, right side"));

    return TRUE;
}

LRESULT CMyDialog::OnNotify(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
        // notification sent by the tooltip to allow text to be set.
        case TTN_GETDISPINFO:
        {
            LPNMTTDISPINFO lpnmtdi = reinterpret_cast<LPNMTTDISPINFO>(lparam);

            if (lpnmtdi->hdr.idFrom == (UINT_PTR)m_edit.GetHwnd())
            {
                m_str = m_edit.GetWindowText();
                if (m_str.IsEmpty()) m_str = "No text to display";
                lpnmtdi->lpszText = const_cast<LPTSTR>(m_str.c_str());
            }
            else if (lpnmtdi->hdr.idFrom == (UINT_PTR)m_richEdit.GetHwnd())
            {
                m_str = m_richEdit.GetWindowText();
                if (m_str.IsEmpty()) m_str = "No text to display";
                lpnmtdi->lpszText = const_cast<LPTSTR>(m_str.c_str());
            }
        }
    }

    return 0;
}

void CMyDialog::OnOK()
{
    MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

BOOL CMyDialog::OnButton()
{
    SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck1()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 1"));
    TRACE("Check Box 1\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck2()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 2"));
    TRACE("Check Box 2\n");
    return TRUE;
}

BOOL CMyDialog::OnCheck3()
{
    SetDlgItemText(IDC_STATIC3, _T("Check Box 3"));
    TRACE("Check Box 3\n");
    return TRUE;
}

BOOL CMyDialog::OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID)
{
    CheckRadioButton(firstID, lastID, clickedID);

    CString str;
    int button = clickedID - firstID + 1;
    str.Format(_T("Radio%d"), button);
    SetDlgItemText(IDC_STATIC3, str);
    TRACE(str); TRACE("\n");

    return TRUE;
}
