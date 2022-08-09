/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (View.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CView class declaration. This class
    defines the appearance and related actions of the DDX/DDV test program.
    This class is a modified version of that found in the FormDocView sample
    distributed with the Win32++ Windows interface classes. The modified sample
    program was based on code provided by Lynn Allan. This program extends the
    given sample by application of Dialog Data Exchange and Validation (DDX/DDV).

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "MyButton.h"
#include "MyScrollbar.h"
#include "Doc.h"

/*============================================================================*/
    class
CView : public CDialog                                                      /*

    Declaration of the CView class of the App-Frame-Doc-View architecture.
*----------------------------------------------------------------------------*/
{
    public:
        CView(UINT nResID);
        virtual ~CView() {}

        void    AdjustStatus();
        HWND    Create(HWND hParent);
        BOOL    GetCheckA() const       { return m_checkVal[0]; }
        BOOL    GetCheckB() const       { return m_checkVal[1]; }
        BOOL    GetCheckC() const       { return m_checkVal[2]; }
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

    private:
        CView(const CView&);                // Disable copy construction
        CView& operator = (const CView&);   // Disable assignment operator

        BOOL    AddToolTip(UINT id);
        BOOL    AddToolTip(UINT id, const CString & s);
        void    AssignToolTips();
        INT_PTR DialogProc(UINT, WPARAM, LPARAM);
        void    DoDataExchange(CDataExchange& DX);
        void    GetDocumentValues();
        void    OnBitmap();
        void    OnButton();
        void    OnCancel() {}   // Suppress esc key closing the dialog
        BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
        INT_PTR OnCtlColor(HDC, HWND, UINT);
        BOOL    OnInitDialog();
        void    OnOK();
        LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
        void    SetControlPositions(int pos);

          // DDX/DDV variables that are connected
        int         m_sliderVal;
        int         m_progressVal;
        int         m_scrollBarVal;
        int         m_focusID;  // the control with current focus
          // DDX/DDV variables for controls on the form
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
        TCHAR       m_LPTSTRVal[256];
        SYSTEMTIME  m_dateSysTime;
        SYSTEMTIME  m_calDateSysTime;

          // controls on the form that need to be attached
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

          // form display background brushes
        CBrush          m_buttonBgBrush;
        CBrush          m_editBgBrush;
        CBrush          m_dialogBgBrush;
        CBrush          m_listBoxBgBrush;
        CBrush          m_scrollBgBrush;
        CBrush          m_staticBoxBgBrush;

          // form tool tips
        CToolTip        m_toolTip;

          // the document
        CDoc            m_doc;
};
/*----------------------------------------------------------------------------*/
#endif //SDI_VIEW_H
