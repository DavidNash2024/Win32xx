/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CView class implementation. This class
    controls the appearance and view-related actions of the DDX/DDV test
    program. This class is a modified version of that found in the
    FormDocView smple distributed with the Win32++ Windows interface classes.
    The modified sample program was based on code provided by LynnAllan. This
    program extends the given sample by application of Dialog Data Exchange
    and Validation (DDX/DDV).

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "Doc.h"
#include "View.h"
#include "App.h"
#include "resource.h"

#ifndef CFM_BACKCOLOR
#define CFM_BACKCOLOR       0x04000000
#endif // CFM_BACKCOLOR

/*=============================================================================*

    Define the color palette                                                */

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

/*============================================================================*/
    CView::
CView(UINT nResID)                                                          /*

    Construct the dialog view based on the resource.rc defined by the
    resource identifier nResID.  Set the initial text foreground and
    background display color and the form background brush for each
    control type, and set the view background color. The frame colors
    are set separately, in CMainFrame::SetThemeColors().
*-----------------------------------------------------------------------------*/
    : CDialog(nResID), m_focusID(0)
{
      // These initial values for the dialog controls that use DDX/DDV
      // validation will be overwritten in OnInitDialog() by the
      // GetDocumentValues() method before they are ever used.  However, it is
      // good practice for constructors to initialize all its essential
      // parameters.
    m_byteVal       = 10;
    m_shortVal      = 0;
    m_intVal        = 0;
    m_UINTVal       = 10;
    m_longVal       = 0L;
    m_ULongVal      = 10;
    m_floatVal      = 0.0;
    m_doubleVal     = 0.0;
    m_LPTSTRVal[0]  = _T('\0');
    m_checkAVal     = FALSE;
    m_checkBVal     = FALSE;
    m_checkCVal     = FALSE;
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
      // buttons
    m_buttonFgClr   = COLOR_BLACK;
    m_buttonBgClr   = COLOR_CYAN;
    m_buttonBgBrush.CreateSolidBrush(COLOR_LT_CYAN);
      // edit boxes
    m_editFgClr     = COLOR_WHITE;
    m_editBgClr     = COLOR_RED;
    m_editBgBrush.CreateSolidBrush(COLOR_LT_RED);
      // the dialog box
    m_dialogFgClr   = COLOR_WHITE;
    m_dialogBgClr   = COLOR_BLUE;
    m_dialogBgBrush.CreateSolidBrush(COLOR_LT_BLUE);
      // list boxes
    m_listBoxFgClr  = COLOR_BLACK;
    m_listBoxBgClr  = COLOR_GREEN;
    m_listBoxBgBrush.CreateSolidBrush(COLOR_LT_GREEN);
      // scroll bars (when used)
    m_scrollFgClr   = COLOR_WHITE;
    m_scrollBgClr   = COLOR_MAGENTA;
    m_scrollBgBrush.CreateSolidBrush(COLOR_LT_MAGENTA);
      // static boxes
    m_staticBoxFgClr = COLOR_BLACK;
    m_staticBoxBgClr = COLOR_YELLOW;
    m_staticBoxBgBrush.CreateSolidBrush(COLOR_LT_YELLOW);
      // richedit controls (these are set differently than the others)
    m_richEditFgClr = COLOR_WHITE;
    m_richEditBgClr = COLOR_RED;
    m_richEditClientBgClr = COLOR_LT_RED;
}

/*============================================================================*/
    BOOL CView::
AddToolTip(HWND hParent, UINT id)                                          /*

    Add the string with the resource id to the control whose resource
    identrifer is also id. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    return AddToolTip(hParent, id, LoadString(id));
}

/*============================================================================*/
    BOOL CView::
AddToolTip(HWND hParent, UINT id, const CString & sToolTip)                /*

    Add the sToolTip string to the control whose resource identrifer is
    id. Return TRUE on success, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    HWND ctl = ::GetDlgItem(hParent, id);
    if (ctl == NULL)
    {
        TRACE(_T("cannot connect tooltip: ") + sToolTip);
        return FALSE;
    }
    if (!m_toolTip.AddTool(ctl, sToolTip.c_str()))
    {
        TRACE(_T("unable to add tooltip: ") + sToolTip);
        return FALSE;
    }
    return TRUE;
}

/*============================================================================*/
    void CView::
AdjustParameters()                                                          /*

    Set the status control to reflect the values of variable controls on the
    dialog.
*-----------------------------------------------------------------------------*/
{
    TCHAR radio[] = {_T('A'), _T('B'), _T('C')};
    LPCTSTR TorF[]   = {_T("false"), _T("true")};

      // display the DateTime control content
    CTime t(m_dateSysTime);
    CString dt = t.Format(_T("%d-%b-%Y %H:%M:%S"));
      // display the MonthCalendar control content
    t = m_calDateSysTime;
    CString mc = t.Format(_T("%d-%b-%Y"));

      // set m_statusBoxVal to display values of all controls
    LPCTSTR fmt = _T("STATUS Button Pressed.\nStatus is:\n\n")
        _T("ID of focus is %d\n")
        _T("BYTE        is %hu\n")
        _T("short       is %hd\n")
        _T("int         is %d\n")
        _T("UINT        is %u\n")
        _T("long        is %ld\n")
        _T("ULONG       is %lu\n")
        _T("float       is %g\n")
        _T("double      is %g\n")
        _T("LPTSTR      is %s\n")
        _T("Text Box    is '%s'\n")
        _T("RichEdit    is '%s'\n")
        _T("ListBox     is '%s'\n")
        _T("ComboBox    is '%s'\n")
        _T("Radio_%c     is selected\n")
        _T("Check Box A is %s\n")
        _T("Check Box B is %s\n")
        _T("Check Box C is %s\n")
        _T("Slider Pos  is %d\n")
        _T("ScrollBar   is %d\n")
        _T("Progress    is %d\n")
        _T("DateTime    is %s\n")
        _T("Month Clndr is %s\n")
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
        m_LPTSTRVal,
        m_editVal.c_str(),
        m_richEditVal.c_str(),
        m_listBoxVal.c_str(),
        m_comboBoxVal.c_str(),
        radio[m_radioA],
        TorF[m_checkAVal],
        TorF[m_checkBVal],
        TorF[m_checkCVal],
        m_sliderVal,
        m_scrollBarVal,
        m_progressVal,
        dt.c_str(),
        mc.c_str()
        );
}

/*============================================================================*/
    void CView::
AssignToolTips()                                                            /*

    Assign tool tips to all controls in the client area.
*-----------------------------------------------------------------------------*/
{
    HWND hParent = *this;
    if (!m_toolTip.Create(hParent))
    {
        TRACE(_T("unable to create tool tips\n"));
        return;
    }
    m_toolTip.AddTool(hParent, _T("Client area"));
    AddToolTip(hParent, IDC_RADIO_A);
    AddToolTip(hParent, IDC_RADIO_B);
    AddToolTip(hParent, IDC_RADIO_C);
    AddToolTip(hParent, IDC_PUSH_ME_BUTTON);
    AddToolTip(hParent, IDC_CHECK_A);
    AddToolTip(hParent, IDC_CHECK_B);
    AddToolTip(hParent, IDC_CHECK_C);
    AddToolTip(hParent, IDOK);
    AddToolTip(hParent, IDC_LISTBOX);
    AddToolTip(hParent, IDC_EDIT_CSTRING);
    AddToolTip(hParent, IDC_EDIT_RICHEDIT);
    AddToolTip(hParent, IDC_STATUS_GROUP);
    AddToolTip(hParent, IDC_EDIT_STATUS);
    AddToolTip(hParent, IDC_ROSE_BITMAP);
    AddToolTip(hParent, IDC_EDIT_BYTE);
    AddToolTip(hParent, IDC_EDIT_SHORT);
    AddToolTip(hParent, IDC_EDIT_INT);
    AddToolTip(hParent, IDC_EDIT_UINT);
    AddToolTip(hParent, IDC_EDIT_LONG);
    AddToolTip(hParent, IDC_EDIT_DWORD);
    AddToolTip(hParent, IDC_EDIT_FLOAT);
    AddToolTip(hParent, IDC_EDIT_DOUBLE);
    AddToolTip(hParent, IDC_EDIT_LPTSTR);
    AddToolTip(hParent, IDC_CALENDAR);
    AddToolTip(hParent, IDC_DATE_TIME);
    AddToolTip(hParent, IDC_SCROLLBAR);
    AddToolTip(hParent, IDC_PROGRESSBAR);
    AddToolTip(hParent, IDC_SLIDER);
    AddToolTip(hParent, IDC_COMBOBOX);
    m_toolTip.Activate(TRUE);
}

/*============================================================================*/
    HWND CView::
Create(HWND hParent = 0)                                                    /*

*-----------------------------------------------------------------------------*/
{
    return DoModeless(hParent);
}

/*============================================================================*/
    INT_PTR CView::
DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)                         /*

    Process the view messages: control colors and owner-drawn buttons.
*-----------------------------------------------------------------------------*/
{
      // Add case statements for each messages to be handled here
    switch (msg)
    {
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORDLG:  // sent to dialog boxes directly, not here!
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
          // for these messages, wparam has the control's hdc and
          // lparam has the control's hwnd
        return OnCtlColor(reinterpret_cast<HDC>(wparam),
            reinterpret_cast<HWND>(lparam), msg);

        case WM_DRAWITEM:
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

        case WM_HSCROLL:
            HWND wnd = reinterpret_cast<HWND>(lparam);
            int pos = 0;
            if (wnd == HWND(m_scrollBar))
                pos = m_scrollBar.GetHScrollPos(msg, wparam, lparam);
            else if (wnd == HWND(m_slider))
                pos = m_slider.GetPos();
            SetControlPositions(pos);
            return TRUE;
    }
      // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}
/*============================================================================*/
    void  CView::
DoDataExchange(CDataExchange& DX)                                           /*

    Specify the controls used for Dialog Data Exchange (DDX) and Dialog Data
    Verification (DDV). This function is invoked each time UpdateData is called.
*-----------------------------------------------------------------------------*/
{
      // DDX_Control is used to attach controls to the numeric IDs
    DX.DDX_Control(IDC_COMBOBOX,        m_comboBox);
    DX.DDX_Control(IDC_LISTBOX,         m_listBox);
    DX.DDX_Control(IDC_EDIT_RICHEDIT,   m_richEdit);
    DX.DDX_Control(IDOK,                m_statusButton);
    DX.DDX_Control(IDC_PUSH_ME_BUTTON,  m_pushButton);
    DX.DDX_Control(IDC_SLIDER,          m_slider);
    DX.DDX_Control(IDC_PROGRESSBAR,     m_progressBar);
    DX.DDX_Control(IDC_SCROLLBAR,       m_scrollBar);
    DX.DDX_Control(IDC_DATE_TIME,       m_dateTime);
    DX.DDX_Control(IDC_CALENDAR,        m_monthCal);

    // The other DDX functions transfer the control's to data to or from
    //  the specified variable.
    // The DDV functions specify the range of valid data for the control.

      // connect byte box to m_byteVal and specify limits
    DX.DDX_Text(IDC_EDIT_BYTE,      m_byteVal);
    DX.DDV_MinMaxByte(              m_byteVal, 10, 200);
      // connect short box to m_shortVal and specify limits
    DX.DDX_Text(IDC_EDIT_SHORT,     m_shortVal);
    DX.DDV_MinMaxShort(             m_shortVal, -1000, 1000);
      // connect int box to m_intVal and specify limits
    DX.DDX_Text(IDC_EDIT_INT,       m_intVal);
    DX.DDV_MinMaxInt(               m_intVal, -10000, 10000);
      // connect UINT box to m_UINTVal and specify limits
    DX.DDX_Text(IDC_EDIT_UINT,      m_UINTVal);
    DX.DDV_MinMaxUInt(              m_UINTVal, 10, 10000);
      // connect long box to m_longVal and specify limits
    DX.DDX_Text(IDC_EDIT_LONG,      m_longVal);
    DX.DDV_MinMaxLong(              m_longVal, -100000, 100000);
    // connect ULONG box to m_ULongVal and specify limits
    DX.DDX_Text(IDC_EDIT_DWORD,     m_ULongVal);
    DX.DDV_MinMaxULong(             m_ULongVal, 10, 100000);
      // connect float box to m_floatVal and specify limits
    DX.DDX_Text(IDC_EDIT_FLOAT,     m_floatVal);
    DX.DDV_MinMaxFloat(             m_floatVal, -10.0, 100000.0);
      // connect double box to m_doubleVal and specify limits
    DX.DDX_Text(IDC_EDIT_DOUBLE,    m_doubleVal);
    DX.DDV_MinMaxDouble(            m_doubleVal, -10.0, 100000.);
      // connect the LPTSTR box and specify length
    DX.DDX_Text(IDC_EDIT_LPTSTR,    m_LPTSTRVal, 255);
    DX.DDV_MaxChars(                m_LPTSTRVal, 25);
      // connect the regular edit box to m_editVal and specify length
    DX.DDX_Text(IDC_EDIT_CSTRING,   m_editVal);
    DX.DDV_MaxChars(                m_editVal, 25);
      // connect the rich edit box to m_richEditVal for string operations
    DX.DDX_Text(IDC_EDIT_RICHEDIT,  m_richEditVal);
    DX.DDV_MaxChars(                m_richEditVal, 25); // limit length
      // connect the slider control to m_sliderVal and specify limits
    DX.DDX_Slider(IDC_SLIDER,       m_sliderVal);
    DX.DDV_MinMaxSlider(            m_sliderVal, 0, 1000);
      // connect the progress bar to m_progressVal
    DX.DDX_Progress(IDC_PROGRESSBAR, m_progressVal);
      // connect scroll bar to m_scrollBarVal
    DX.DDX_Scroll(  IDC_SCROLLBAR,  m_scrollBarVal);
      // connect the radio boxes
    DX.DDX_Radio( IDC_RADIO_A,      m_radioA);
      // connect check boxes
    DX.DDX_Check(IDC_CHECK_A,       m_checkAVal);
    DX.DDX_Check(IDC_CHECK_B,       m_checkBVal);
    DX.DDX_Check(IDC_CHECK_C,       m_checkCVal);
      // Note: Data in a combo box may be accessed two ways: by string or by
      // index. Only one of these two methods should be used.  Comment the
      // other out below.
    DX.DDX_CBString(IDC_COMBOBOX,   m_comboBoxVal); // use string method
//  DDX_CBIndex(IDC_COMBOBOX,       m_comboBoxIndx); // use index method
//  m_comboBox.GetLBText(m_comboBoxIndx, m_comboBoxVal.GetBuffer(256));
//  m_comboBoxVal.ReleaseBuffer();

      // Note: Data in a list box may be accessed two ways: by string or by
      // index. Only one of these two methods should be used.  Comment the
      // other out below.
    DX.DDX_LBString(IDC_LISTBOX,    m_listBoxVal);  // use string method
//  DDX_LBIndex(IDC_LISTBOX,        m_listBoxIndx);  // use index method
//  m_listBox.GetText(m_listBoxIndx, m_listBoxVal.GetBuffer(256));
//  m_listBoxVal.ReleaseBuffer();

      // engage the DateTime control and specify +/-30 days
    CTime tnow = CTime::GetCurrentTime();
    CTimeSpan tmo  = 30 * 86400;
    CTime     tMin = tnow - tmo,
              tMax = tnow + tmo;
    SYSTEMTIME stMin, stMax;
    tMin.GetAsSystemTime(stMin);
    tMax.GetAsSystemTime(stMax);
    DX.DDX_DateTime(IDC_DATE_TIME, m_dateSysTime);
    DX.DDV_MinMaxDateTime(m_dateSysTime,  stMin, stMax);
      // engage the Month Calendar control and specify +/-30 days
    DX.DDX_MonthCal(IDC_CALENDAR, m_calDateSysTime);
    DX.DDV_MinMaxMonth(m_calDateSysTime,  stMin, stMax);
      // connect the status box
    DX.DDX_Text(IDC_EDIT_STATUS,    m_statusBoxVal);
}

/*============================================================================*/
    void CView::
GetDocumentValues()                                                         /*

    Load those things from the document that it saves.
*-----------------------------------------------------------------------------*/
{
    m_byteVal       = TheDoc().GetByte();
    m_shortVal      = TheDoc().GetShort();
    m_intVal        = TheDoc().GetInt();
    m_UINTVal       = TheDoc().GetUINT();
    m_longVal       = TheDoc().GetLong();
    m_ULongVal      = TheDoc().GetULong();
    m_floatVal      = TheDoc().GetFloat();
    m_doubleVal     = TheDoc().GetDouble();
    m_radioA        = TheDoc().GetRadio();
    m_checkAVal     = TheDoc().GetCheckA();
    m_checkBVal     = TheDoc().GetCheckB();
    m_checkCVal     = TheDoc().GetCheckC();
    m_editVal       = TheDoc().GetEditBox();
    m_richEditVal   = TheDoc().GetRichEditBox();
    m_listBoxVal    = TheDoc().GetListBoxS();
    m_listBoxIndx   = TheDoc().GetListBoxX();
    m_comboBoxVal   = TheDoc().GetComboBoxS();
    m_comboBoxIndx  = TheDoc().GetComboBoxX();
    m_sliderVal     = TheDoc().GetSlider();
    m_dateSysTime   = TheDoc().GetDateTime();
    m_calDateSysTime = TheDoc().GetMoCalendar();
    StrCopy(m_LPTSTRVal, TheDoc().GetLPTSTR(), 256);
}

/*============================================================================*/
    void CView::
OnBitmap()                                                                  /*

    Activate the DDX/DDV mechanism to read current values from the dialog
    form and to set the status box to indicate this button was pressed.
*-----------------------------------------------------------------------------*/
{
      // save current contents of controls

    UpdateDialog(READFROMCONTROL);
      // reset status to just this message:
    m_statusBoxVal = _T("The moondance rose.");
      // post the message
    UpdateDialog(SENDTOCONTROL);
    TRACE("Bitmap Pressed\n");
}

/*============================================================================*/
    void CView::
OnButton()                                                                  /*

    Activate the DDX/DDV mechanism to read current values from the dialog
    form and to set the status box to indicate this button was pressed.
*-----------------------------------------------------------------------------*/
{
      // read current contents of controls to memory
    UpdateDialog(READFROMCONTROL);
      // reset the status message to just this:
    m_statusBoxVal = _T("PUSH ME button Pressed");
      // send this status message (and also the other values) back into
      // the control
    UpdateDialog(SENDTOCONTROL);
    TRACE("PUSH ME button Pressed\n");
}

/*============================================================================*/
    BOOL CView::
OnCommand(WPARAM wparam, LPARAM lparam)                                  /*

    The framework calls this member function when the user selects an item
    from a menu, when a child control sends a notification message, or when
    an accelerator keystroke is translated.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);
    WORD id  = LOWORD(wparam);
    WORD ctl = HIWORD(wparam);
    switch (id)
    {
    case IDC_PUSH_ME_BUTTON:
        OnButton();
        m_focusID = id;
        return TRUE;

    case IDC_ROSE_BITMAP:
        OnBitmap();
        m_focusID = id;
        return TRUE;

    case IDC_EDIT_STATUS:
        m_focusID = id;
        return TRUE;


    case IDC_RADIO_A:
    case IDC_RADIO_B:
    case IDC_RADIO_C:
        SetRadioA(id - IDC_RADIO_A);
        UpdateDialog(SENDTOCONTROL);
        return TRUE;

    case IDC_CHECK_A:
        SetCheckA( !GetCheckA() );
        UpdateDialog(SENDTOCONTROL);
        return TRUE;

    case IDC_CHECK_B:
        SetCheckB( !GetCheckB() );
        UpdateDialog(SENDTOCONTROL);
        return TRUE;

    case IDC_CHECK_C:
        SetCheckC( !GetCheckC() );
        UpdateDialog(SENDTOCONTROL);
        return TRUE;
    }
      // deal with setting the focus for edit controls, combo boxes,
      // list boxes, radio buttons, and check boxes
    if (ctl == EN_SETFOCUS || ctl == CBN_SETFOCUS || ctl == LBN_SETFOCUS
        || (ctl == BN_SETFOCUS && IDC_RADIO_A <= id && id <= IDC_CHECK_C))
    {
        m_focusID = id;
        return FALSE;
    }
    return FALSE;
}

/*============================================================================*/
    INT_PTR  CView::
OnCtlColor(HDC dc, HWND hWnd, UINT nCtlColor)                               /*

    This member function is invoked when a child control is about to be
    drawn. It can be used to prepare the display context dc for drawing
    the control using user-selected colors.
*-----------------------------------------------------------------------------*/
{
      // get the control numeric ID, when needed
    UINT id = ::GetDlgCtrlID(hWnd);
      // get the display context
    CDC dcCtl(dc);
      // to avoid compiler warnings, declare control colors and
      // assign them arbitrary (because unused) values
    COLORREF fg = COLOR_BLACK;
    COLORREF bk = COLOR_BLACK;
    CBrush   br = CBrush(COLOR_BLACK);
      // handle each color message separtely
    switch (nCtlColor)
    {
        case WM_CTLCOLORBTN:
              // handle each button separately (but here, they have the
              // same colors)
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
              // change caption color of group box
            if (id == IDC_STATUS_GROUP)
            {
                fg = m_dialogFgClr;
                bk = m_dialogBgClr;
                br = m_dialogBgBrush; // doesn't do anything
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
    }
      // set the foreground and background device contexts
    dcCtl.SetTextColor(fg);
    dcCtl.SetBkColor(bk);
      // return the brush handle
    return (INT_PTR)(HBRUSH)br;
}

/*============================================================================*/
    BOOL CView::
OnInitDialog()                                                              /*

    This method is activated before the dialog box just before is displayed.
    Insert instructions that are needed to perform special processing when the
    dialog box is initialized.  The return value is always TRUE.
*-----------------------------------------------------------------------------*/
{
      // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);
      // add tool tips to controls in client area
    AssignToolTips();
      // Connect controls to IDs and read default data values into them.
    UpdateDialog(SENDTOCONTROL);
      // Set the rich edit control text foreground and background colors
      // and the control background color.  This is needed only once (not
      // like other controls set in OnCtlColor()).
    CHARFORMAT2 chf;
    chf.cbSize = sizeof(chf);
    chf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
    chf.dwEffects = 0;
    chf.crTextColor = m_richEditFgClr;
    chf.crBackColor = m_richEditBgClr;
    m_richEdit.SetDefaultCharFormat(chf);
    m_richEdit.SetBackgroundColor(FALSE, m_richEditClientBgClr);
      // initialize some of the subclassed controls for this test:
      // list box, combo box, slider, progress bar, and scroll bar:
      // put some text in the list box (normally, entries might be
      // retrieved from the document, saved in the registry, but here,
      // for simplplicity, we just enter some sample values)
    m_listBox.ResetContent();
    for (int i = 0 ; i < 10 ; i++)
    {
        CString s;
        s.Format(_T("List Box%d"), i);
        m_listBox.AddString(s);
    }
      // ditto for the combo box
    m_comboBox.ResetContent();
    for (int j = 0 ; j < 8 ; j++)
    {
        CString s;
        s.Format(_T("Combo Box%d"), j);
        m_comboBox.AddString(s);
    }
      // set the slider, progress, and scroll bar ranges
    int lo = 0;
    int page = 20;
    int hi   = 50 * page;
    m_scrollBar.SetScrollInfo(lo, hi, 0, page);
      // set slider range and page size
    m_slider.SetRangeMin(lo);
    m_slider.SetRangeMax(hi);
    m_slider.SetPageSize(page);
    m_progressBar.SetRange((short)lo, (short)hi);
      // set Date-Time display format
    m_dateTime.SetFormat(_T("dd'-'MMM'-'yyyy' 'HH':'mm':'ss"));
      // retrieve values from the document
    GetDocumentValues();
      // gang together progress, scroll bar, and slider
    m_progressVal = m_scrollBarVal = m_sliderVal;
    return TRUE;
}

/*============================================================================*/
    void CView::
OnOK()                                                                      /*

    Called when the user clicks the button with an ID of IDOK.  Display the
    contents of all controls and save those that are part of the document.
*-----------------------------------------------------------------------------*/
{
    UpdateDialog(READFROMCONTROL);
    AdjustParameters();
    UpdateDialog(SENDTOCONTROL);
    UpdateParameters();

    TRACE("STATUS Button Pressed.\n\n");
}

/*============================================================================*/
    LRESULT CView::
OnNotify(WPARAM wparam, LPARAM lparam)                                  /*

    Respond to WM_NOTIFY messages received from child windows. Here, typical
    notifications from the data-time and month calendar controls are identified.
*-----------------------------------------------------------------------------*/
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
    }
    return FALSE;
}

/*============================================================================*/
    void    CView::
SetControlPositions(int pos)                                                       /*

*-----------------------------------------------------------------------------*/
{
    SetSlider(pos);
    SetScrollBar(pos);
    SetProgress(pos);
    UpdateDialog(SENDTOCONTROL);
    SetFocusID(IDC_SCROLLBAR);
    AdjustParameters();
}

/*============================================================================*/
    BOOL    CView::
UpdateDialog(BOOL bReadFromControl)                                         /*

    Update data items in memory (when bReadFromControl is READFROMCONTROL) or
    in the dialog controls (when bReadFromControl is SENDTOCONTROL). The list
    of affected controls is specified in the DoDataExchange() method.
*-----------------------------------------------------------------------------*/
{
    CDataExchange DX;
    return UpdateData(DX, bReadFromControl);
}

/*============================================================================*/
    void CView::
UpdateParameters()                                                          /*

    Save the pertinent members of the view that are also members of the
    document.
*-----------------------------------------------------------------------------*/
{
    TheDoc().SetByte(m_byteVal);
    TheDoc().SetShort(m_shortVal);
    TheDoc().SetInt(m_intVal);
    TheDoc().SetUINT(m_UINTVal);
    TheDoc().SetLong(m_longVal);
    TheDoc().SetULong(m_ULongVal);
    TheDoc().SetFloat(m_floatVal);
    TheDoc().SetDouble(m_doubleVal);
    TheDoc().SetLPTSTR(m_LPTSTRVal);
    TheDoc().SetEditBox(m_editVal);
    TheDoc().SetRichEditBox(m_richEditVal);
    TheDoc().SetListBoxS(m_listBoxVal);
    TheDoc().SetListBoxX(m_listBoxIndx);
    TheDoc().SetComboBoxS(m_comboBoxVal);
    TheDoc().SetComboBoxX(m_comboBoxIndx);
    TheDoc().SetRadio(m_radioA);
    TheDoc().SetCheckA(m_checkAVal);
    TheDoc().SetCheckB(m_checkBVal);
    TheDoc().SetCheckC(m_checkCVal);
    TheDoc().SetSlider(m_sliderVal);
    TheDoc().SetDateTime(m_dateSysTime);
    TheDoc().SetMoCalendar(m_calDateSysTime);
}
/*----------------------------------------------------------------------------*/