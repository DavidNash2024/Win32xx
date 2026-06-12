/////////////////////////////////////////
// View.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "Doc.h"
#include "View.h"
#include "App.h"
#include "resource.h"

static const  CString unit[] = {L'A', L'B', L'C'};

// Define the color palette.
#define COLOR_BLACK     RGB(  0,   0,   0)
#define COLOR_GRAY      RGB(100, 100, 100)
#define COLOR_LT_GRAY   RGB(200, 200, 200)
#define COLOR_WHITE     RGB(255, 255, 255)

#define COLOR_BLUE      RGB(  0,   0, 255)
#define COLOR_LT_BLUE   RGB(200, 200, 255)
#define COLOR_DK_BLUE   RGB(  0,   0, 128)
#define COLOR_LILAC     RGB(230, 230, 255)

#define COLOR_BROWN     RGB(255, 128,  64)
#define COLOR_DK_BROWN  RGB(128,  64,   0)

#define COLOR_YELLOW    RGB(255, 255,   0)
#define COLOR_LT_YELLOW RGB(255, 255, 200)
#define COLOR_DK_YELLOW RGB(128, 128,   0)

#define COLOR_CYAN      RGB(  0, 255, 255)
#define COLOR_LT_CYAN   RGB(200, 255, 255)
#define COLOR_DK_CYAN   RGB(  0, 128, 128)

#define COLOR_GREEN     RGB(  0, 255,   0)
#define COLOR_LT_GREEN  RGB(200, 255, 200)
#define COLOR_DK_GREEN  RGB(  0, 128,   0)

#define COLOR_MAGENTA   RGB(255,   0, 255)
#define COLOR_LT_MAGENTA RGB(255, 200, 255)
#define COLOR_DK_MAGENTA RGB(128,   0, 128)

#define COLOR_RED       RGB(255,   0,   0)
#define COLOR_LT_RED    RGB(255, 200, 200)
#define COLOR_DK_RED    RGB(128,   0,   0)

//////////////////////////////
// CView function definitions.
//

// Construct the dialog used as the frame's view window.
CView::CView(UINT resID) : CDialog(resID), m_focusID(0)
{
    m_byteVal       = 10;
    m_shortVal      = 0;
    m_intVal        = 0;
    m_UINTVal       = 10;
    m_longVal       = 0L;
    m_ULongVal      = 10;
    m_floatVal      = 0.0;
    m_doubleVal     = 0.0;
    m_LPWSTRVal[0]  = L'\0';
    m_checkVal[0]     = FALSE;
    m_checkVal[1]     = FALSE;
    m_checkVal[2]     = FALSE;
    m_radioA        = 0;
    m_editVal.Empty();
    m_richEditVal.Empty();
    m_listBoxVal.Empty();
    m_listBoxIndx   = 0;
    m_comboBoxVal.Empty();
    m_comboBoxIndx  = 0;
    m_sliderVal     = 0;
    m_progressVal   = 0;
    m_scrollBarVal  = 0;
    CTime t = CTime::GetCurrentTime();
    t.GetAsSystemTime(m_dateSysTime);
    m_calDateSysTime = m_dateSysTime;

    // Buttons.
    m_buttonFgClr   = COLOR_BLACK;
    m_buttonBgClr   = COLOR_CYAN;
    m_buttonBgBrush.CreateSolidBrush(COLOR_LT_CYAN);

    // Edit boxes.
    m_editFgClr     = COLOR_WHITE;
    m_editBgClr     = COLOR_RED;
    m_editBgBrush.CreateSolidBrush(COLOR_LT_RED);

     // The dialog box colors.
    m_dialogFgClr   = COLOR_WHITE;
    m_dialogBgClr   = COLOR_BLUE;
    m_dialogBgBrush.CreateSolidBrush(COLOR_LILAC);

    // The list box colors.
    m_listBoxFgClr  = COLOR_BLACK;
    m_listBoxBgClr  = COLOR_GREEN;
    m_listBoxBgBrush.CreateSolidBrush(COLOR_LT_GREEN);

    // The scroll bar colors.
    m_scrollFgClr   = COLOR_WHITE;
    m_scrollBgClr   = COLOR_MAGENTA;
    m_scrollBgBrush.CreateSolidBrush(COLOR_LT_MAGENTA);

    // The static box colors.
    m_staticBoxFgClr = COLOR_BLACK;
    m_staticBoxBgClr = COLOR_YELLOW;
    m_staticBoxBgBrush.CreateSolidBrush(COLOR_LT_YELLOW);

    // The Richedit colors.
    m_richEditFgClr = COLOR_WHITE;
    m_richEditBgClr = COLOR_RED;
    m_richEditClientBgClr = COLOR_LT_RED;
}

// Add the string with the specified resource id to the control with the
// specified resource id.
BOOL CView::AddToolTip(UINT id)
{
    return AddToolTip(id, LoadString(id));
}

// Add the specified tooltip string to the control with the specified
// resource id.
BOOL CView::AddToolTip(UINT id, const CString & tooltip)
{
    HWND ctl = ::GetDlgItem(*this, id);
    if (ctl == nullptr)
    {
        TRACE("cannot connect tooltip: " + tooltip);
        return FALSE;
    }
    if (!m_toolTip.AddTool(ctl, tooltip.c_str()))
    {
        TRACE("unable to add tooltip: " + tooltip);
        return FALSE;
    }
    return TRUE;
}

// Set the status control to reflect the values of variable controls on the
// dialog.
void CView::AdjustStatus()
{
    LPCWSTR TorF[]   = {L"OFF", L"ON"};

    // Display the DateTime control content.
    CTime t(m_dateSysTime);
    CString dt = t.Format(L"%d-%b-%Y %H:%M:%S");

    // Display the MonthCalendar control content.
    t = m_calDateSysTime;
    CString mc = t.Format(L"%d-%b-%Y");

    // Set m_statusBoxVal to display values of all controls.
    LPCWSTR fmt = L"STATUS Button Pressed.\nStatus is:\n\n"
        L"ID of focus is %d\n"
        L"BYTE        is %hu\n"
        L"short       is %hd\n"
        L"int         is %d\n"
        L"UINT        is %u\n"
        L"long        is %ld\n"
        L"ULONG       is %lu\n"
        L"float       is %g\n"
        L"double      is %g\n"
        L"LPWSTR      is %s\n"
        L"Text Box    is '%s'\n"
        L"RichEdit    is '%s'\n"
        L"ListBox     is '%s'\n"
        L"ComboBox    is '%s'\n"
        L"Radio_%s     is selected\n"
        L"Check Box A is %s\n"
        L"Check Box B is %s\n"
        L"Check Box C is %s\n"
        L"Slider Pos  is %d\n"
        L"ScrollBar   is %d\n"
        L"Progress    is %d\n"
        L"DateTime    is %s\n"
        L"Month Clndr is %s\n"
        ;
    m_statusBoxVal.Format(fmt,
        m_focusID,
        m_byteVal,
        m_shortVal,
        m_intVal,
        m_UINTVal,
        m_longVal,
        m_ULongVal,
        m_floatVal,
        m_doubleVal,
        m_LPWSTRVal,
        m_editVal.c_str(),
        m_richEditVal.c_str(),
        m_listBoxVal.c_str(),
        m_comboBoxVal.c_str(),
        unit[m_radioA].c_str(),
        TorF[m_checkVal[0]],
        TorF[m_checkVal[1]],
        TorF[m_checkVal[2]],
        m_sliderVal,
        m_scrollBarVal,
        m_progressVal,
        dt.c_str(),
        mc.c_str()
        );
}


// Assign tool tips to all controls in the client area.
void CView::AssignToolTips()
{
    m_toolTip.Create(*this);
    m_toolTip.AddTool(*this, L"Client area");

    AddToolTip(IDC_RADIO_A);
    AddToolTip(IDC_RADIO_B);
    AddToolTip(IDC_RADIO_C);
    AddToolTip(IDC_PUSH_ME_BUTTON);
    AddToolTip(IDC_CHECK_A);
    AddToolTip(IDC_CHECK_B);
    AddToolTip(IDC_CHECK_C);
    AddToolTip(IDOK);
    AddToolTip(IDC_LISTBOX);
    AddToolTip(IDC_EDIT_CSTRING);
    AddToolTip(IDC_EDIT_RICHEDIT);
    AddToolTip(IDC_STATUS_GROUP);
    AddToolTip(IDC_EDIT_STATUS);
    AddToolTip(IDC_ROSE_BITMAP);
    AddToolTip(IDC_EDIT_BYTE);
    AddToolTip(IDC_EDIT_SHORT);
    AddToolTip(IDC_EDIT_INT);
    AddToolTip(IDC_EDIT_UINT);
    AddToolTip(IDC_EDIT_LONG);
    AddToolTip(IDC_EDIT_DWORD);
    AddToolTip(IDC_EDIT_FLOAT);
    AddToolTip(IDC_EDIT_DOUBLE);
    AddToolTip(IDC_EDIT_LPWSTR);
    AddToolTip(IDC_CALENDAR);
    AddToolTip(IDC_DATE_TIME);
    AddToolTip(IDC_SCROLLBAR);
    AddToolTip(IDC_PROGRESSBAR);
    AddToolTip(IDC_SLIDER);
    AddToolTip(IDC_COMBOBOX);

    m_toolTip.Activate(TRUE);
}

// This function processes the dialog messages.
INT_PTR CView::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:    return OnCtlColor(msg, wparam, lparam);

        case WM_DRAWITEM:          return OnDrawItem(wparam, lparam);
        case WM_HSCROLL:           return OnHScroll(wparam, lparam);

        // Do default processing for other messages.
        default: return DialogProcDefault(msg, wparam, lparam);;
        }
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Specify the controls used for Dialog Data Exchange(DDX) and Dialog Data
// Verification(DDV). This function is invoked each time UpdateData is called.
void CView::DoDataExchange(CDataExchange& dx)
{
    // DDX_Control is used to attach controls to the numeric ID.
    dx.DDX_Control(IDC_COMBOBOX,        m_comboBox);
    dx.DDX_Control(IDC_LISTBOX,         m_listBox);
    dx.DDX_Control(IDC_EDIT_RICHEDIT,   m_richEdit);
    dx.DDX_Control(IDOK,                m_statusButton);
    dx.DDX_Control(IDC_PUSH_ME_BUTTON,  m_pushButton);
    dx.DDX_Control(IDC_SLIDER,          m_slider);
    dx.DDX_Control(IDC_PROGRESSBAR,     m_progressBar);
    dx.DDX_Control(IDC_SCROLLBAR,       m_scrollBar);
    dx.DDX_Control(IDC_DATE_TIME,       m_dateTime);
    dx.DDX_Control(IDC_CALENDAR,        m_monthCal);

    // The other DDX functions transfer the control's to data to or from
    //  the specified variable.
    // The DDV functions specify the range of valid data for the control.

    // Connect byte box to m_byteVal and specify limits.
    dx.DDX_Text(IDC_EDIT_BYTE,      m_byteVal);
    dx.DDV_MinMaxByte(              m_byteVal, 10, 200);

    // Connect short box to m_shortVal and specify limits.
    dx.DDX_Text(IDC_EDIT_SHORT,     m_shortVal);
    dx.DDV_MinMaxShort(             m_shortVal, -1000, 1000);

    // Connect int box to m_intVal and specify limits.
    dx.DDX_Text(IDC_EDIT_INT,       m_intVal);
    dx.DDV_MinMaxInt(               m_intVal, -10000, 10000);

    // Connect UINT box to m_UINTVal and specify limits.
    dx.DDX_Text(IDC_EDIT_UINT,      m_UINTVal);
    dx.DDV_MinMaxUInt(              m_UINTVal, 10, 10000);

    // Connect long box to m_longVal and specify limits.
    dx.DDX_Text(IDC_EDIT_LONG,      m_longVal);
    dx.DDV_MinMaxLong(              m_longVal, -100000, 100000);

    // Connect ULONG box to m_ULongVal and specify limits.
    dx.DDX_Text(IDC_EDIT_DWORD,     m_ULongVal);
    dx.DDV_MinMaxULong(             m_ULongVal, 10, 100000);

    // Connect float box to m_floatVal and specify limits.
    dx.DDX_Text(IDC_EDIT_FLOAT,     m_floatVal);
    dx.DDV_MinMaxFloat(             m_floatVal, -10.0, 100000.0);

    // Connect double box to m_doubleVal and specify limits.
    dx.DDX_Text(IDC_EDIT_DOUBLE,    m_doubleVal);
    dx.DDV_MinMaxDouble(            m_doubleVal, -10.0, 100000.);

    // Connect the LPWSTR box and specify length.
    dx.DDX_Text(IDC_EDIT_LPWSTR,    m_LPWSTRVal, 255);
    dx.DDV_MaxChars(                m_LPWSTRVal, 25);

    // Connect the regular edit box to m_editVal and specify length.
    dx.DDX_Text(IDC_EDIT_CSTRING,   m_editVal);
    dx.DDV_MaxChars(                m_editVal, 25);

    // Connect the rich edit box to m_richEditVal for string operations.
    dx.DDX_Text(IDC_EDIT_RICHEDIT,  m_richEditVal);
    dx.DDV_MaxChars(                m_richEditVal, 25); // Limit length.

    // Connect the slider control to m_sliderVal and specify limits.
    dx.DDX_Slider(IDC_SLIDER,       m_sliderVal);
    dx.DDV_MinMaxSlider(            m_sliderVal, 0, 1000);

    // Connect the progress bar to m_progressVal.
    dx.DDX_Progress(IDC_PROGRESSBAR, m_progressVal);

    // Connect scroll bar to m_scrollBarVal.
    dx.DDX_Scroll(IDC_SCROLLBAR,    m_scrollBarVal);

    // Connect the radio boxes.
    dx.DDX_Radio( IDC_RADIO_A,      m_radioA);

    // Connect the check boxes.
    dx.DDX_Check(IDC_CHECK_A,       m_checkVal[0]);
    dx.DDX_Check(IDC_CHECK_B,       m_checkVal[1]);
    dx.DDX_Check(IDC_CHECK_C,       m_checkVal[2]);

    // Note: Data in a combo box may be accessed two ways: by string or by
    // index. Only one of these two methods should be used.  Comment the
    // other out below.
    dx.DDX_CBString(IDC_COMBOBOX,   m_comboBoxVal);  // Use string method.
//  dx.DDX_CBIndex(IDC_COMBOBOX,    m_comboBoxIndx); // Use index method.
//  m_comboBox.GetLBText(m_comboBoxIndx, m_comboBoxVal.GetBuffer(256));
//  m_comboBoxVal.ReleaseBuffer();

    // Note: Data in a list box may be accessed two ways: by string or by
    // index. Only one of these two methods should be used.  Comment the
    // other out below.
    dx.DDX_LBString(IDC_LISTBOX,    m_listBoxVal);   // Use string method.
//  dx.DDX_LBIndex(IDC_LISTBOX,     m_listBoxIndx);  // Use index method.
//  m_listBox.GetText(m_listBoxIndx, m_listBoxVal.GetBuffer(256));
//  m_listBoxVal.ReleaseBuffer();

    // Engage the DateTime control and specify +/-30 days.
    CTime tnow = CTime::GetCurrentTime();
    CTimeSpan tmo  = 30 * 86400;
    CTime     tMin = tnow - tmo,
              tMax = tnow + tmo;
    SYSTEMTIME stMin, stMax;
    tMin.GetAsSystemTime(stMin);
    tMax.GetAsSystemTime(stMax);
    dx.DDX_DateTime(IDC_DATE_TIME, m_dateSysTime);
    dx.DDV_MinMaxDateTime(m_dateSysTime,  stMin, stMax);
      // engage the Month Calendar control and specify +/-30 days
    dx.DDX_MonthCal(IDC_CALENDAR, m_calDateSysTime);
    dx.DDV_MinMaxMonth(m_calDateSysTime,  stMin, stMax);
      // connect the status box
    dx.DDX_Text(IDC_EDIT_STATUS,    m_statusBoxVal);
}

// Retrieve the values saved in the document.
void CView::GetDocumentValues()
{
    m_byteVal       = m_doc.GetByte();
    m_shortVal      = m_doc.GetShort();
    m_intVal        = m_doc.GetInt();
    m_UINTVal       = m_doc.GetUINT();
    m_longVal       = m_doc.GetLong();
    m_ULongVal      = m_doc.GetULong();
    m_floatVal      = m_doc.GetFloat();
    m_doubleVal     = m_doc.GetDouble();
    m_radioA        = m_doc.GetRadio();
    m_checkVal[0]   = m_doc.GetCheckA();
    m_checkVal[1]   = m_doc.GetCheckB();
    m_checkVal[2]   = m_doc.GetCheckC();
    m_editVal       = m_doc.GetEditBox();
    m_richEditVal   = m_doc.GetRichEditBox();
    m_listBoxVal    = m_doc.GetListBoxS();
    m_listBoxIndx   = m_doc.GetListBoxX();
    m_comboBoxVal   = m_doc.GetComboBoxS();
    m_comboBoxIndx  = m_doc.GetComboBoxX();
    m_sliderVal     = m_doc.GetSlider();
    m_dateSysTime   = m_doc.GetDateTime();
    m_calDateSysTime = m_doc.GetMoCalendar();
    StrCopy(m_LPWSTRVal, m_doc.GetLPWSTR(), 256);
}

// Activates the DDX/DDV mechanism to read the values from the dialog's
// controls, and update the Status Window to indicate the PUSHME button
// was pressed.
BOOL CView::OnPushMeButton(int id)
{
    // Read current contents of the dialog's controls to memory.
    UpdateData(m_dx, READFROMCONTROL);

    // Reset the status message to just this:
    m_statusBoxVal = L"PUSH ME button Pressed";
    m_focusID = id;
    TRACE("PUSH ME button Pressed\n");

    // Updates the values of the dialog's controls.
    UpdateData(m_dx, SENDTOCONTROL);

    return TRUE;
}

// Respond to the press of the Check A button.
BOOL CView::OnCheckAButton()
{
    UpdateData(m_dx, READFROMCONTROL);
    m_statusBoxVal << "Check A set " << (m_checkVal[0] ? "ON." : "OFF.");
    UpdateData(m_dx, SENDTOCONTROL);

    return TRUE;
}

// Respond to the press of the Check B button.
BOOL CView::OnCheckBButton()
{
    UpdateData(m_dx, READFROMCONTROL);
    m_statusBoxVal << "Check B set " << (m_checkVal[1] ? "ON." : "OFF.");
    UpdateData(m_dx, SENDTOCONTROL);

    return TRUE;
}

// Respond to the press of the Check C button.
BOOL CView::OnCheckCButton()
{
    UpdateData(m_dx, READFROMCONTROL);
    m_statusBoxVal << "Check C set " << (m_checkVal[2] ? "ON." : "OFF.");
    UpdateData(m_dx, SENDTOCONTROL);

    return TRUE;
}

// This is called when a button in the dialog is pressed.
BOOL CView::OnCommand(WPARAM wparam, LPARAM)
{
    // Update the ID of the control that currently has focus.
    WORD ctl = HIWORD(wparam);
    WORD id = LOWORD(wparam);
    if (ctl == EN_SETFOCUS || ctl == CBN_SETFOCUS || ctl == LBN_SETFOCUS
        || (ctl == BN_SETFOCUS && IDC_RADIO_A <= id && id <= IDC_CHECK_C))
    {
        m_focusID = id;
        return TRUE;
    }

    switch (id)
    {
    case IDC_PUSH_ME_BUTTON:  return OnPushMeButton(id);
    case IDC_ROSE_BITMAP:     return OnRoseBitmap(id);

    case IDC_RADIO_A:
    case IDC_RADIO_B:
    case IDC_RADIO_C:         return OnRadioButton();

    case IDC_CHECK_A:         return OnCheckAButton();
    case IDC_CHECK_B:         return OnCheckBButton();
    case IDC_CHECK_C:         return OnCheckCButton();

    default: return FALSE;
    }
}

// This function is called before a child control is drawn. It is used to
// prepare the window to draw with the predefined colors.
INT_PTR CView::OnCtlColor(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Get the control numeric ID.
    HWND hwnd = reinterpret_cast<HWND>(lparam);
    UINT id = ::GetDlgCtrlID(hwnd);

    // Get the display context.
    HDC hdc = reinterpret_cast<HDC>(wparam);
    CDC dcCtl(hdc);

    COLORREF fg = COLOR_BLACK;
    COLORREF bk = COLOR_BLACK;
    CBrush   br;

    switch (msg)
    {
    case WM_CTLCOLORBTN:
        if (id == IDOK)
        {
            fg = m_buttonFgClr;
            bk = m_buttonBgClr;
            br = m_buttonBgBrush;
        }
        if (id == IDC_PUSH_ME_BUTTON)
        {
            fg = m_buttonFgClr;
            bk = m_buttonBgClr;
            br = m_buttonBgBrush;
        }
        break;

    case WM_CTLCOLOREDIT:
        fg = m_editFgClr;
        bk = m_editBgClr;
        br = m_editBgBrush;
        break;

    case WM_CTLCOLORDLG:
        fg = m_dialogFgClr;
        bk = m_dialogBgClr;
        br = m_dialogBgBrush;
        break;

    case WM_CTLCOLORLISTBOX:
        fg = m_listBoxFgClr;
        bk = m_listBoxBgClr;
        br = m_listBoxBgBrush;
        break;

    case WM_CTLCOLORSCROLLBAR:
        fg = m_scrollFgClr;
        bk = m_scrollBgClr;
        br = m_scrollBgBrush;
        break;

    case WM_CTLCOLORSTATIC:
        // Change caption color of group box.
        if (id == IDC_STATUS_GROUP)
        {
            fg = m_dialogFgClr;
            bk = m_dialogBgClr;
            br = m_dialogBgBrush;
        }
        else if (IDC_RADIO_A <= id && id <= IDC_CHECK_C)
        {
            fg = m_buttonFgClr;
            bk = m_buttonBgClr;
            br = m_buttonBgBrush;
        }
        else
        {
            fg = m_staticBoxFgClr;
            bk = m_staticBoxBgClr;
            br = m_staticBoxBgBrush;
        }
        break;

    default: break;
    }

    // Set the foreground and background device contexts.
    dcCtl.SetTextColor(fg);
    dcCtl.SetBkColor(bk);

    // Keep changes to the device context
    dcCtl.Detach();

    // Return the brush handle
    return (INT_PTR)br.GetHandle();
}

// This function performs the drawing of the owner-drawn buttons.
INT_PTR CView::OnDrawItem(WPARAM wparam, LPARAM lparam)
{
    LPDRAWITEMSTRUCT lpDrawItemStruct =
        reinterpret_cast<LPDRAWITEMSTRUCT>(lparam);

    UINT id = static_cast<UINT>(wparam);
    if (id == IDOK)
        m_statusButton.DrawItem(lpDrawItemStruct);

    if (id == IDC_PUSH_ME_BUTTON)
        m_pushButton.DrawItem(lpDrawItemStruct);

    return TRUE;
}

// This funcion is called when the scroll position of the scrollbar or
// progress bar has changed.
INT_PTR CView::OnHScroll(WPARAM wparam, LPARAM lparam)
{
    HWND wnd = reinterpret_cast<HWND>(lparam);
    int pos = 0;
    if (wnd == m_scrollBar)
        pos = m_scrollBar.GetHScrollPos(wparam, lparam);
    else if (wnd == m_slider)
        pos = m_slider.GetPos();

    SetControlPositions(pos);
    return TRUE;
}

// This method is called before the dialog is displayed. Here we initialize DDX
// and set up the dialog's controls.
BOOL CView::OnInitDialog()
{
    // Set the Icon.
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Add tool tips to controls in client area.
    AssignToolTips();

    // Connect controls to IDs and read default data values into them.
    UpdateData(m_dx, SENDTOCONTROL);

    // Set the rich edit control text foreground and background colors and the
    // control background color.
    CHARFORMAT2 chf;
    chf.cbSize = sizeof(chf);
    chf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
    chf.dwEffects = 0;
    chf.crTextColor = m_richEditFgClr;
    chf.crBackColor = m_richEditBgClr;
    m_richEdit.SetDefaultCharFormat(chf);
    m_richEdit.SetBackgroundColor(FALSE, m_richEditClientBgClr);

    // Initialize the list box, combo box, slider, progress bar, and scrollbar.
    m_listBox.ResetContent();
    for (int i = 0 ; i < 10 ; i++)
    {
        CString s;
        s.Format(L"List Box%d", i);
        m_listBox.AddString(s);
    }

    // Initialize the combo box.
    m_comboBox.ResetContent();
    for (int j = 0 ; j < 8 ; j++)
    {
        CString s;
        s.Format(L"Combo Box%d", j);
        m_comboBox.AddString(s);
    }

    // Set the slider, progress, and scrollbar ranges.
    int lo = 0;
    int page = 20;
    int hi   = 50 * page;
    m_scrollBar.SetScrollInfo(lo, hi, 0, page);

    // Set slider range and page size.
    m_slider.SetRangeMin(lo);
    m_slider.SetRangeMax(hi);
    m_slider.SetPageSize(page);
    m_progressBar.SetRange((short)lo, (short)hi);

    // Set Date-Time display format.
    m_dateTime.SetFormat(L"dd'-'MMM'-'yyyy' 'HH':'mm':'ss");

    // Retrieve values from the document.
    GetDocumentValues();

    // Gang together progress, scroll bar, and slider.
    m_progressVal = m_sliderVal;
    m_scrollBarVal = m_sliderVal;
    return TRUE;
}

// Called when the user clicks STATUS button. It updates the text displayed
// in the Status Window.
void CView::OnOK()
{
    UpdateData(m_dx, READFROMCONTROL);
    AdjustStatus();
    UpdateData(m_dx, SENDTOCONTROL);
    UpdateDocument();
    TRACE("STATUS Button Pressed.\n\n");
}

// Respond to WM_NOTIFY messages from the time and month calendar controls.
LRESULT CView::OnNotify(WPARAM wparam, LPARAM lparam)
{
    NMHDR *pNMHdr = reinterpret_cast<LPNMHDR>(lparam);
    switch (pNMHdr->code)
    {
    case NM_SETFOCUS:   // date-time picker
    case MCN_SELECT:    // month calendar
    case MCN_SELCHANGE: //  "       "
    {
        m_focusID = static_cast<int>(wparam);
        return TRUE;
    }

    default: return FALSE;
    }
}

// Respond to the press of a radio button.
BOOL CView::OnRadioButton()
{
    UpdateData(m_dx, READFROMCONTROL);
    m_statusBoxVal = CString("Radio button set to ") + unit[m_radioA];
    UpdateData(m_dx, SENDTOCONTROL);

    return TRUE;
}

// Respond to a press of the rose bitmap.
BOOL CView::OnRoseBitmap(int id)
{
    // Save current contents of controls.
    UpdateData(m_dx, READFROMCONTROL);

    // Reset status to just this message:
    m_statusBoxVal = L"The moondance rose.";
    TRACE("Bitmap Pressed\n");
    m_focusID = id;

    // Post the message.
    UpdateData(m_dx, SENDTOCONTROL);
    TRACE("Bitmap Pressed\n");

    return TRUE;
}

// Toggle the check box buttons.
void CView::SetCheck(UINT check)
{
    // Toggle the check control.
    m_checkVal[check] = (m_checkVal[check]? FALSE : TRUE);

    // Set the status.
    m_statusBoxVal = "Check " + unit[check] + " set " +
        (m_checkVal[check] ? "ON." : "OFF.");

    UpdateData(m_dx, SENDTOCONTROL);
}

// Set the slider, scroll, and progress bars to the specified position.
void CView::SetControlPositions(int pos)
{
    SetSlider(pos);
    SetScrollBar(pos);
    SetProgress(pos);
    AdjustStatus();

    // Update the slider, scrollbar, progressbar and status windows.
    UpdateData(m_dx, SENDTOCONTROL);
}

// Set the pressed radio button.
void CView::SetRadio(UINT id)
{
    m_radioA = id;
    m_statusBoxVal = CString("Radio button set to ") + unit[m_radioA];
    UpdateData(m_dx, SENDTOCONTROL);
}

// Save the dialog's data in the document.
void CView::UpdateDocument()
{
    m_doc.SetByte(m_byteVal);
    m_doc.SetShort(m_shortVal);
    m_doc.SetInt(m_intVal);
    m_doc.SetUINT(m_UINTVal);
    m_doc.SetLong(m_longVal);
    m_doc.SetULong(m_ULongVal);
    m_doc.SetFloat(m_floatVal);
    m_doc.SetDouble(m_doubleVal);
    m_doc.SetLPWSTR(m_LPWSTRVal);
    m_doc.SetEditBox(m_editVal);
    m_doc.SetRichEditBox(m_richEditVal);
    m_doc.SetListBoxS(m_listBoxVal);
    m_doc.SetListBoxX(m_listBoxIndx);
    m_doc.SetComboBoxS(m_comboBoxVal);
    m_doc.SetComboBoxX(m_comboBoxIndx);
    m_doc.SetRadio(m_radioA);
    m_doc.SetCheckA(m_checkVal[0]);
    m_doc.SetCheckB(m_checkVal[1]);
    m_doc.SetCheckC(m_checkVal[2]);
    m_doc.SetSlider(m_sliderVal);
    m_doc.SetDateTime(m_dateSysTime);
    m_doc.SetMoCalendar(m_calDateSysTime);
}

// Update the dialog's controls and save their values in the document.
// Return TRUE on success, FALSE on failure.
BOOL CView::UpdateDialog(BOOL bReadFromControl)
{
    if (bReadFromControl == SENDTOCONTROL)
        AdjustStatus();

    BOOL ok = UpdateData(m_dx, bReadFromControl);
    if (ok)
    {   // all is well
        TRACE("Verification passed\n");
        UpdateDocument();
    }
    else
    {
        TRACE("*** Verification failed ***\n");
    }

    return ok;
}
