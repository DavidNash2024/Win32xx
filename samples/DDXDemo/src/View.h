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

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "MyButton.h"
#include "MyScrollbar.h"
#include "MyMonthCalendar.h"
#include "MyDateTime.h"
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

        void    AdjustParameters();
        HWND    Create(HWND hParent);
        int     GetCheckA() const       { return m_checkAVal; }
        int     GetCheckB() const       { return m_checkBVal; }
        int     GetCheckC() const       { return m_checkCVal; }
        void    SetCheckA(int value)    { m_checkAVal = value; }
        void    SetCheckB(int value)    { m_checkBVal = value; }
        void    SetCheckC(int value)    { m_checkCVal = value; }
        void    SetFocusID(int value)   { m_focusID = value; }
        void    SetProgress(int value)  { m_progressVal = value; }
        void    SetRadioA(int value)    { m_radioA = value; }
        void    SetScrollBar(int value) { m_scrollBarVal = value; }
        void    SetSlider(int value)    { m_sliderVal = value; }
        CDoc&   TheDoc()                { return m_doc; }
        BOOL    UpdateDialog(BOOL bReadFromControl);
        void    UpdateParameters();

    private:
        BOOL    AddToolTip(HWND, UINT id);
        BOOL    AddToolTip(HWND, UINT id, const CString & s);
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
          // nested classes for some of this dialog's child windows
          // that require no changes from the base class
          // (nesting is optional--it's done here to keep the IDE's
          // class view tidy)
        class CMyComboBox      : public CComboBox      {};
        class CMyListBox       : public CListBox       {};
        class CMyProgressBar   : public CProgressBar   {};
        class CMyRichEdit      : public CRichEdit      {};
        class CMySlider        : public CSlider        {};

        // DDX/DDV variables that need to be visible to friends
        int         m_sliderVal;
        int         m_progressVal;
        int         m_scrollBarVal;
        int         m_focusID;  // the control with current focus
          // DDX/DDV variables for controls on the form
        BYTE        m_byteVal;
        short       m_shortVal;
        UINT        m_UINTVal;
        long        m_longVal;
        DWORD       m_ULongVal;
        float       m_floatVal;
        double      m_doubleVal;
        int         m_intVal;
        int         m_checkAVal;
        int         m_checkBVal;
        int         m_checkCVal;
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
        CMyComboBox      m_comboBox;
        CMyDateTime      m_dateTime;
        CMyListBox       m_listBox;
        CMyMonthCalendar m_monthCal;
        CMyProgressBar   m_progressBar;
        CMyRichEdit      m_richEdit;
        CMyScrollBar     m_scrollBar;
        CMyButton        m_statusButton;
        CMyButton        m_pushButton;
        CMySlider        m_slider;

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

          // form display backgound brushes
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
