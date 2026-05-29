/////////////////////////////////////////
// View.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "MyButton.h"
#include "MyScrollbar.h"
#include "Doc.h"


////////////////////////////////////////////////////////
// The CView class manages the view of the application. It is derived
// from CDialog, and is responsible for the dialog's appearance and
// view related actions.
class CView : public CDialog
{
public:
    CView(UINT nResID);
    virtual ~CView() override = default;

    void    AdjustStatus();
    BOOL    GetCheckA() const       { return m_checkVal[0]; }
    BOOL    GetCheckB() const       { return m_checkVal[1]; }
    BOOL    GetCheckC() const       { return m_checkVal[2]; }
    int     GetRadio() const        { return m_radioA; }
    void    SetCheck(UINT);
    void    SetRadio(UINT);
    void    SetCheckAStatus();
    void    SetCheckBStatus();
    void    SetCheckCStatus();
    void    SetFocusID(int value)   { m_focusID = value; }
    void    SetProgress(int value)  { m_progressVal = value; }
    void    SetRadioAStatus();
    void    SetScrollBar(int value) { m_scrollBarVal = value; }
    void    SetSlider(int value)    { m_sliderVal = value; }
    CDoc&   TheDoc()                { return m_doc; }
    BOOL    UpdateDialog(BOOL bReadFromControl);
    void    UpdateDocument();

protected:
    virtual void    DoDataExchange(CDataExchange& dx) override;
    virtual BOOL    OnInitDialog() override;
    virtual void    OnOK() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM) override;
    virtual void    OnCancel() override {}   // Suppress esc key closing the dialog.
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    BOOL    AddToolTip(UINT id);
    BOOL    AddToolTip(UINT id, const CString & s);
    void    AssignToolTips();
    void    GetDocumentValues();
    void    OnBitmap();
    void    OnButton();
    INT_PTR OnCtlColor(HDC, HWND, UINT);
    void    SetControlPositions(int pos);

    // DDX/DDV variables that are connected.
    int         m_sliderVal;
    int         m_progressVal;
    int         m_scrollBarVal;
    int         m_focusID;  // The control with current focus.

    // DDX/DDV variables for controls on the form.
    CDataExchange m_dx;
    BYTE        m_byteVal;
    short       m_shortVal;
    UINT        m_UINTVal;
    long        m_longVal;
    DWORD       m_ULongVal;
    float       m_floatVal;
    double      m_doubleVal;
    int         m_intVal;
    BOOL        m_checkVal[3];
    int         m_comboBoxIndx;
    int         m_listBoxIndx;
    int         m_radioA;
    CString     m_comboBoxVal;
    CString     m_listBoxVal;
    CString     m_editVal;
    CString     m_richEditVal;
    CString     m_statusBoxVal;
    WCHAR       m_LPWSTRVal[256];
    SYSTEMTIME  m_dateSysTime;
    SYSTEMTIME  m_calDateSysTime;

    // Controls on the form that need to be attached.
    CComboBox      m_comboBox;
    CDateTime      m_dateTime;
    CListBox       m_listBox;
    CMonthCalendar m_monthCal;
    CProgressBar   m_progressBar;
    CRichEdit      m_richEdit;
    CMyScrollBar   m_scrollBar;
    CMyButton      m_statusButton;
    CMyButton      m_pushButton;
    CSlider        m_slider;

    // form control display colors
    COLORREF        m_buttonFgClr;
    COLORREF        m_buttonBgClr;
    COLORREF        m_editFgClr;
    COLORREF        m_editBgClr;
    COLORREF        m_richEditFgClr;
    COLORREF        m_richEditBgClr;
    COLORREF        m_richEditClientBgClr;
    COLORREF        m_dialogFgClr;
    COLORREF        m_dialogBgClr;
    COLORREF        m_listBoxFgClr;
    COLORREF        m_listBoxBgClr;
    COLORREF        m_scrollFgClr;
    COLORREF        m_scrollBgClr;
    COLORREF        m_staticBoxFgClr;
    COLORREF        m_staticBoxBgClr;

    // Form display background brushes.
    CBrush          m_buttonBgBrush;
    CBrush          m_editBgBrush;
    CBrush          m_dialogBgBrush;
    CBrush          m_listBoxBgBrush;
    CBrush          m_scrollBgBrush;
    CBrush          m_staticBoxBgBrush;

    // Form tool tips.
    CToolTip        m_toolTip;

    // The document.
    CDoc            m_doc;
};

#endif //SDI_VIEW_H
