/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (View.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The DDXDemo CView class declaration. This class
	defines the appearance and related actions of the DDX/DDV test program.
	This class is a modified version of that found in the FormDocView sample
	distributed with the Win32++ Windows interface classes, Copyright (c)
	2005-2015 David Nash, used under permissions granted therein. The
	modified sample program was based on code provided by Lynn Allan. This
	program extends the given sample by application of Dialog Data Exchange
	and Validation (DDX/DDV).

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
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

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Declaration of the CView class

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H


#include "MyButton.h"
#include "MySlider.h"
#include "MyScrollbar.h"
#include "MyMonthCalendar.h"
#include "MyDateTime.h"

class CMyDataExchange : public CDataExchange
{
public:
	CMyDataExchange() : CDataExchange() 	{}
	
	// ToDo:
	// Add additional DDX or DDV functions here
};


  // forward reference
class CMainFrame;

/*============================================================================*/
	class
CView

/*----------------------------------------------------------------------------*/
	: public CDialog
{
	friend class CMainFrame;
	friend class CMySlider;
	friend class CMyScrollBar;

	public:
		CView(UINT nResID);
		virtual ~CView();
		virtual HWND 	Create(HWND hParent);
			BOOL	UpdateDialog(BOOL bReadFromControl
				    = SENDTOCONTROL);

	protected:
		virtual BOOL    AddToolTip(HWND, UINT nID);
		virtual BOOL    AddToolTip(HWND, UINT nID, const CString & s);
		virtual void	AdjustParameters();
		virtual void    AssignToolTips();
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
		virtual void 	DoDataExchange(CDataExchange& DX);
		virtual void    GetDocumentValues();
		virtual BOOL 	OnCommand(WPARAM wParam, LPARAM lParam);
		virtual INT_PTR OnCtlColor(HDC, HWND, UINT);
		virtual BOOL 	OnInitDialog();
		virtual void 	OnOK();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual BOOL 	PreTranslateMessage(MSG& Msg);  
		virtual void 	UpdateParameters();

		  // DDX/DDV variables that need to be visible to friends
		int		m_iSlider,
				m_iProgress,
				m_iScrollBar;
		int    		m_nIDFocus;  // the control with current focus

	private:
		void OnBitmap();
		void OnButton();

		  // nested classes for some of this dialog's child windows
		  // that require no changes from the base class
		  // (nesting is optional--it's done here to keep the IDE's
		  // class view tidy)
		class CMyComboBox      : public CComboBox      {};
		class CMyListBox       : public CListBox       {};
		class CMyProgressBar   : public CProgressBar   {};
		class CMyRichEdit      : public CRichEdit      {};

		  // DDX/DDV variables for controls on the form
		BYTE            m_iByte;
		short           m_iShort;
		UINT            m_iUINT;
		long            m_iLong;
		DWORD           m_ULong;
		float           m_fFloat;
		double          m_dDouble;
		int             m_iInt,
				m_iCheckA,
				m_iCheckB,
				m_iCheckC,
				m_iComboBox,
				m_iListBox,
				m_iRadioA;
		CString         m_sComboBox,
				m_sListBox,
				m_sString,
				m_sRichEdit,
				m_sStatus;
		TCHAR           m_LPTSTR[256];
		SYSTEMTIME      m_stDateTime,
				m_stMoCalendar;
		
		  // controls on the form that need to be attached
		CMyComboBox      m_ComboBox;
		CMyDateTime      m_DateTime;
		CMyListBox 	 m_ListBox;
		CMyMonthCalendar m_MoCalendar;
		CMyProgressBar   m_Progress;
		CMyRichEdit      m_RichEdit;
		CMyScrollBar     m_ScrollBar;
		CMySlider        m_Slider;
		CMyButton        m_StatusButton,
				 m_PushButton;

		  // form display colors
		COLORREF        m_clrCtlBtnFg,
				m_clrCtlBtnBg,
				m_clrCtlEditFg,
				m_clrCtlEditBg,
				m_clrCtlRichEditFg,
				m_clrCtlRichEditBg,
				m_clrCtlRichEditBk,
				m_clrCtlDlgFg,
				m_clrCtlDlgBg,
				m_clrCtlLBFg,
				m_clrCtlLBBg,
				m_clrCtlScrlFg,
				m_clrCtlScrlBg,
				m_clrCtlStatFg,
				m_clrCtlStatBg;

		  // form display backgound brushes
		CBrush          m_brCtlBtnBg,
				m_brCtlEditBg,
				m_brCtlDlgBg,
				m_brCtlLBBg,
				m_brCtlScrlBg,
				m_brCtlStatBg;

		  // form tool tips
		CToolTip        m_ToolTip;
};
/*----------------------------------------------------------------------------*/
#endif //SDI_VIEW_H
