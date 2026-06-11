/////////////////////////////////////////
// View.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "MyButton.h"
#include "MyScrollbar.h"
#include "Doc.h"


/////////////////////////////////////////////////////////////
// CView manages dialog used as the main frame's view window.
//
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
    void    SetFocusID(int value)   { m_focusID = value; }
    void    SetProgress(int value)  { m_progressVal = value; }
    void    SetScrollBar(int value) { m_scrollBarVal = value; }
    void    SetSlider(int value)    { m_sliderVal = value; }
    void    SetRadio(UINT);
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
    BOOL    OnCheckAButton();
    BOOL    OnCheckBButton();
    BOOL    OnCheckCButton();
    INT_PTR OnCtlColor(UINT msg, WPARAM wparam, LPARAM lparam);
    INT_PTR OnDrawItem(WPARAM wparam, LPARAM lparam);
    INT_PTR OnHScroll(WPARAM wparam, LPARAM lparam);
    BOOL    OnPushMeButton(int id);
    BOOL    OnRadioButton();
    BOOL    OnRoseBitmap(int id);
    void    SetControlPositions(int pos);

    // DDX/DDV variables for controls in the dialog.
    BYTE       m_byteVal;
    BOOL       m_checkVal[3];
    double     m_doubleVal;
    float      m_floatVal;
    long       m_longVal;
    WCHAR      m_LPWSTRVal[256];
    short      m_shortVal;
    UINT       m_UINTVal;
    DWORD      m_ULongVal;

    int        m_comboBoxIndx;
    int        m_intVal;
    int        m_listBoxIndx;
    int        m_progressVal;
    int        m_radioA;
    int        m_sliderVal;
    int        m_scrollBarVal;

    CString    m_comboBoxVal;
    CString    m_editVal;
    CString    m_listBoxVal;
    CString    m_richEditVal;
    CString    m_statusBoxVal;
   
    SYSTEMTIME m_calDateSysTime;
    SYSTEMTIME m_dateSysTime;

    // Controls on the form that are attached to CWnd objects.
    CComboBox      m_comboBox;
    CDateTime      m_dateTime;
    CListBox       m_listBox;
    CMonthCalendar m_monthCal;
    CProgressBar   m_progressBar;
    CMyButton      m_pushButton;
    CRichEdit      m_richEdit;
    CMyScrollBar   m_scrollBar;
    CSlider        m_slider;
    CMyButton      m_statusButton;

    // Form control colors.
    COLORREF m_buttonBgClr;
    COLORREF m_buttonFgClr;
    COLORREF m_dialogBgClr;
    COLORREF m_dialogFgClr;
    COLORREF m_editBgClr;
    COLORREF m_editFgClr;
    COLORREF m_listBoxBgClr;
    COLORREF m_listBoxFgClr;
    COLORREF m_richEditBgClr;
    COLORREF m_richEditClientBgClr;
    COLORREF m_richEditFgClr;
    COLORREF m_scrollBgClr;
    COLORREF m_scrollFgClr;
    COLORREF m_staticBoxBgClr;
    COLORREF m_staticBoxFgClr;

    // Form background brushes.
    CBrush   m_buttonBgBrush;
    CBrush   m_dialogBgBrush;
    CBrush   m_editBgBrush;
    CBrush   m_listBoxBgBrush;
    CBrush   m_scrollBgBrush;
    CBrush   m_staticBoxBgBrush;

    CDataExchange m_dx;  // The DDV/DDX data exchange object.
    CToolTip m_toolTip;  // The dialog's tooltips control.
    CDoc m_doc;          // The document that store's the dialog's values.
    int  m_focusID;      // The control with current focus.
};

#endif //SDI_VIEW_H
