// Win32++   Version 8.1 beta
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// Acknowledgement:
//
// The original author of CDataExchange is:
//
//      Robert C. Tausworthe
//      email: robert.c.tausworthe@ieee.org
//
////////////////////////////////////////////////////////



#ifndef _WIN32XX_DDX_H_
#define _WIN32XX_DDX_H_

#include <float.h>
#include <iomanip>
#include "wxx_controls.h"


namespace Win32xx
{

	///////////////////////////////////////////////////////////////////////////
	//
	//	Definitions
	//
	//////////////////////////////////////////////////////////////////////////

	  // DDX read/write direction (easier to remember)
	const BOOL SENDTOCONTROL = FALSE;	// set variable values into controls
	const BOOL READFROMCONTROL = TRUE;	// get values from controls into variables

	// modal error message notification
	inline void	ErrorMessageBox(LPCTSTR msg)
	{
		::MessageBox(NULL, msg, _T("Error"), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//	Declaration of the CDataExchange class.
	//
	///////////////////////////////////////////////////////////////////////////

	class CDataExchange
	//	A class for for data exchange and validation
	{

	public:
		  // Constructor/destructor
		CDataExchange(CWnd& dlgWnd, BOOL bReadFromControl, BOOL allowDDXDDV = TRUE);
		~CDataExchange();

		void Fail();

		  // simple text operations
		void DDX_Text(int nIDC, BYTE& value);
		void DDX_Text(int nIDC, short& value);
		void DDX_Text(int nIDC, int& value);
		void DDX_Text(int nIDC, UINT& value);
		void DDX_Text(int nIDC, long& value);
		void DDX_Text(int nIDC, DWORD& value);
		void DDX_Text(int nIDC, CString& value);
		void DDX_Text(int nIDC, LPTSTR value, int nMaxLen);
		void DDX_Text(int nIDC, float& value, int precision = FLT_DIG);
		void DDX_Text(int nIDC, double& value, int precision = DBL_DIG);

		  // special control types
		void DDX_Check(int nIDC, int& value);
		void DDX_Radio(int nIDC, int& value);
		void DDX_LBString(int nIDC, CString& value);
		void DDX_CBString(int nIDC, CString& value);
		void DDX_LBIndex(int nIDC, int& index);
		void DDX_CBIndex(int nIDC, int& index);
		void DDX_LBStringExact(int nIDC, CString& value);
		void DDX_CBStringExact(int nIDC, CString& value);
		void DDX_Scroll(int nIDC, int& value);
		void DDX_Slider(int nIDC, int& value);
		void DDX_Progress(int nIDC, int& value);
		void DDX_MonthCal(int nIDC, SYSTEMTIME& value);
		void DDX_DateTime(int nIDC, SYSTEMTIME& value);

		  // special control subclassing
		void DDX_Control(int nIDC, CWnd& rCtl);

		  // Standard Dialog Data Validation (DDV) routines
		void DDV_MinMaxByte(BYTE value, BYTE minVal, BYTE maxVal);
		void DDV_MinMaxShort(short value, short minVal, short maxVal);
		void DDV_MinMaxInt(int value, int minVal, int maxVal);
		void DDV_MinMaxLong(long value,  long minVal, long maxVal);
		void DDV_MinMaxUInt(UINT value, UINT minVal, UINT maxVal);
		void DDV_MinMaxULong(DWORD value, DWORD minVal, DWORD maxVal);
		void DDV_MinMaxFloat(float const& value, float minVal, float maxVal, int precision = FLT_DIG);
		void DDV_MinMaxDouble(double const& value,double minVal, double maxVal, int precision = DBL_DIG);
		void DDV_MaxChars(CString const& value, int nChars);
		void DDV_MinMaxSlider(DWORD value, DWORD minVal, DWORD maxVal);
		void DDV_MinMaxDateTime(SYSTEMTIME&, const SYSTEMTIME&, const SYSTEMTIME&);
		void DDV_MinMaxMonth(SYSTEMTIME&, const SYSTEMTIME&, const SYSTEMTIME&);

		  // helper operations
		HWND PrepareCtrl(int nIDC);   // return HWND of control
		HWND PrepareEditCtrl(int nIDC); // record this is an edit

//	private:
			 // data members
		int   m_nID;               // ID of last-accessed control
		HWND  m_hWndLastControl;   // handle of last-accessed control
		HWND  m_hWndLastEditControl; // handle of last edit control
		BOOL  m_bEditLastControl;  // most recent control is an edit box
		BOOL  m_bReadFromControl;  // TRUE means read and validate data
		BOOL  m_allowDDXDDV;       // observe DDX and DDV exceptions
		HWND  m_hWndParent;        // parent dialog

	};


	inline CString GetWindowText(HWND hWndCtrl);
	inline ULONGLONG SystemTimeToULL(const SYSTEMTIME &systime);


}	// namespace Win32xx


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	  // exceptions
	const int   USER_EXCEPTION          = 1;       // exception of type int

	  // DDX anomaly prompting messages
	static LPCTSTR DDX_MSG_BYTE   = _T("Please enter an integer between 0 and 255.");
	static LPCTSTR DDX_MSG_INT    = _T("Please enter an integer.");
	static LPCTSTR DDX_MSG_LONG   = _T("Please enter a long integer.");
	static LPCTSTR DDX_MSG_SHORT  = _T("Please enter a short integer.");
	static LPCTSTR DDX_MSG_REAL   = _T("Please enter a number.");
	static LPCTSTR DDX_MSG_UINT   = _T("Please enter a positive integer.");
	static LPCTSTR DDX_MSG_ULONG  = _T("Please enter a positive long integer.");

	  // DDV formats and prompts
//	static LPCTSTR DDV_MSG_DATETIME     = _T("Please enter a date and/or time.");
	static LPCTSTR DDV_MSG_INT_RANGE    = _T("Please enter an integer in (%ld, %ld).");
	static LPCTSTR DDV_MSG_UINT_RANGE   = _T("Please enter an integer in (%lu, %lu).");
//	static LPCTSTR DDV_MSG_RADIO_BUTTON = _T("Please select a button.");
	static LPCTSTR DDV_MSG_REAL_RANGE   = _T("Please enter a number in (%.*g, %.*g).");
	static LPCTSTR DDV_MSG_STRING_SIZE  = _T("%s\n is too long.\nPlease enter no ")\
						 _T("more than %ld characters.");
//	static LPCTSTR DDV_MSG_NUMERIC  = _T("Please enter a valid number.");
//	static LPCTSTR DDV_MSG_SLIDER  = _T("Slider position not in the range (%ld, %ld).");


#ifndef __BORLANDC__    // required for Borland 5.5 support
	//============================================================================
	inline tStringStream& operator>>(tStringStream& ts, BYTE& value)
	{
		UINT u = 0;
		ts >> u;
		if (u < 256)
		{
			value = (BYTE)u;
		}
		else
		{
			ts.setstate(std::ios_base::failbit);
		}
		return ts;
	}
#endif


	////////////////////////////////////////////////////////////////
	//
	//	CDataExchange Class Methods
	//
	////////////////////////////////////////////////////////////////

	//============================================================================
	inline CDataExchange::CDataExchange(CWnd& dlgWnd, BOOL bReadFromControl, BOOL allowDDXDDV /* = TRUE */)
	//	Construct a DDX-DDV object, where pDlgWnd is the parent window
	//	containing controls for data exchange and validation. If allowDDXDDV is
	//	TRUE, then observe exceptions generated when a DDX_ or DDV_ method
	//	would otherwise detect a proscribed condition. This may be useful in
	//	start-up situations where uninitialized values are present before a
	//	window opens.
	{
		  // the window has to be valid
		assert(dlgWnd.IsWindow());	// Check this

		  // record the default action and parent window
		m_bReadFromControl = bReadFromControl;
		m_hWndParent       = dlgWnd;
		m_hWndLastControl  = NULL;
		m_allowDDXDDV      = allowDDXDDV;
	}

	//============================================================================
	inline CDataExchange::~CDataExchange()
	//	Destructor.
	{
	}


	//============================================================================
	inline void CDataExchange::Fail()
	//	This function is called whenever a failure condition has been detected
	//	while validating the value in a control and a throw is required to
	//	inhibit further DoDataExchange() activity.  Normally, the focus of
	//	the parent dialog is given to the control window causing the failure
	//	condition and, in the case of an edit control, the characters in that
	//	control will be selected.
	{
		if (!m_bReadFromControl)
		{
			TRACE(_T("Warning: CDataExchange::Fail() called while ")
				_T("writing to a control.\n"));
			// and throw a user exception, below
		}
		else if (m_hWndLastControl != NULL)
		{
			  // if the offender is an edit control
			if (m_bEditLastControl)
			{
				  // select all characters in the edit item
				::SetFocus(m_hWndLastControl);
				::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
			}
		}
		else
		{         // merely announce
			TRACE(_T("Error: validation failed with no control to ")
				_T("restore focus to.\n"));
		}

		throw USER_EXCEPTION;
	}

	//============================================================================
	inline HWND CDataExchange::PrepareCtrl(int nIDC)
	//	Find the handle to the control whose numeric identifier is nID and
	//	record this as the last control handle encountered, set the last-edit
	//	member to a false default value.
	{
		assert(nIDC != 0);
		assert(nIDC != -1);	// not allowed

		HWND	hWndCtrl = ::GetDlgItem(m_hWndParent, nIDC);
		assert(hWndCtrl);

		m_hWndLastControl  = hWndCtrl;
		m_bEditLastControl = FALSE; // not an edit item by default
		return hWndCtrl;
	}

	//============================================================================
	inline HWND CDataExchange::PrepareEditCtrl(int nIDC)
	//	Get and record the handle corresponding to nIDC and set the edit-last
	//	boolean to true. This is to be used only for edit controls.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		assert(hWndCtrl);

		m_bEditLastControl = true;
		return hWndCtrl;
	}


	//============================================================================
	// Retrieves the from a control and returns a CString.
	inline CString GetWindowText(HWND hWndCtrl)
	{
		int nLength = ::GetWindowTextLength(hWndCtrl);
		CString str;

		::GetWindowText(hWndCtrl, str.GetBuffer(nLength), nLength+1);
		str.ReleaseBuffer();

		return str;
	}


	////////////////////////////////////////////////////////////////
	//
	//	DDX: Data Exchange Procedures
	//
	////////////////////////////////////////////////////////////////

	//============================================================================
	inline void CDataExchange::DDX_Control(int nIDC, CWnd& rCtl)
	//	This function enables the transfer of data between a control in a
	//	dialog box, form view, or control view object and a CWnd data member
	//	rCtl having the control nID, usingthe DDX/DDV object m_DX.
	//	Specifically, this method attaches the control to the nID. If it
	//	has already been subclassed, the member does nothing.
	{
		if (!rCtl.IsWindow())    // not subclassed yet
		{
			HWND hWndCtrl = PrepareCtrl(nIDC);
			assert(hWndCtrl);
			rCtl.Attach(hWndCtrl);
			  // If the control has reparented itself (e.g.,
			  // invisible control), make sure that the CWnd gets
			  // properly wired to its control site.
			if (m_hWndParent != ::GetParent(rCtl))
				rCtl.Attach(m_hWndParent);
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, BYTE& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type BYTE.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			BYTE oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_BYTE);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << (int)value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, short& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type short.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			short oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_SHORT);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, int& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type int.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			int oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_INT);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, UINT& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type UINT.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			UINT oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_UINT);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, long& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type long.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			long oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_LONG);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, ULONG& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type ULONG.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			ULONG oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_ULONG);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, float& value, int precision /* = FLT_DIG */)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type float with the given precision.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			float oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_REAL);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << std::setprecision(precision) << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, double& value, int precision /* = DBL_DIG */)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type double with the given precision.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			double oldvalue = value;
			CString str = GetWindowText(hWndCtrl);
			tStringStream ts(str.c_str());
			ts >> value;
			if (ts.fail())
			{
				ErrorMessageBox(DDX_MSG_REAL);
				value = oldvalue;
				Fail();
			}
		}
		else
		{
			tStringStream tss;
			tss << std::setprecision(precision) << value;
			::SetWindowText(hWndCtrl, tss.str().c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, CString& value)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type CString.
	{
		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			value = GetWindowText(hWndCtrl);
		}
		else
		{
			::SetWindowText(hWndCtrl, value.c_str());
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Text(int nIDC, LPTSTR value, int nMaxLen)
	//	Perform a text box data exchange on the current DDX/DDV object m_DX with
	//	data value of type LPTSTR with the given maximum length.
	{
		assert(value);

		if (!m_allowDDXDDV)
			return;

		HWND hWndCtrl = PrepareEditCtrl(nIDC);
		if (m_bReadFromControl)
		{
			CString str = GetWindowText(hWndCtrl);
			lstrcpyn(value, str, nMaxLen-1);
			value[nMaxLen-1] = _T('\0');
		}
		else
		{
			::SetWindowText(hWndCtrl, (LPTSTR)value);
		}
	}


	////////////////////////////////////////////////////////////////
	//
	//	Data exchange for special controls
	//
	////////////////////////////////////////////////////////////////

	//============================================================================
	inline void CDataExchange::DDX_Check(int nIDC, int& value)
	//	Perform a check box control data exchange on the DDX/DDV object m_DX
	//	with the control numbered nIDC to and from the given int value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
		{
			value = (int)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L);
			assert(value == BST_CHECKED || value == BST_UNCHECKED ||
				value == BST_INDETERMINATE);
		}
		else
		{
			if (value < 0 || value > 2)
			{
				CString str = CString(_T("Warning: dialog data checkbox value "))
								+ value + _T(" out of range.\n");
				TRACE(str);
				value = 0;  // set default to off
			}
			::SendMessage(hWndCtrl, BM_SETCHECK, (WPARAM)value, 0L);
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_Radio(int nIDC, int& value)
	//	Perform a data exchange for the state of a group of auto radio buttons
	//	on the DDX/DDV object m_DX with the control numbered nIDC to and from
	//	the given int value, where the identifier nID is that of the first
	//	control in the group.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		  // assure that the control is a radio button and part of a group
		BOOL firstInGroup = (BOOL)(::GetWindowLongPtr(hWndCtrl, GWL_STYLE) &WS_GROUP);
		assert(firstInGroup);
		  // assure the button is a radio button
		BOOL isRadioButton = (BOOL)(::SendMessage(hWndCtrl,
			WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON);
		assert(isRadioButton);

		  // preset the returned value to empty in case no button is set
		if (m_bReadFromControl)
			value = -1;

		  // traverse all buttons in the group: we've already established
		  // there's a group, so set up for the radio buttons in the group
		firstInGroup = FALSE;
		for (int iButton = 0; hWndCtrl != NULL && !firstInGroup; )
		{
			if (isRadioButton)
			{	  // this control in the group is a radio button:
				  // if asked to read the control,
				if (m_bReadFromControl)
				{         // is this button set?
					if (::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L) != 0)
					{         // Record the value the first time,
						  // but if it happens again, there
						  // is an error--only one button
						  // checked is allowed.
						assert(value == -1);
						value = iButton;
					}
				}
				else // if asked to select the radio button,
				{
					  // then select it
					::SendMessage(hWndCtrl, BM_SETCHECK, (iButton == value), 0L);
				}
				iButton++;
			}
			else
			{
				TRACE(_T("Warning: there is a non-radio button in ")
					_T("a radio button group.\n"));
			}
			  // check the next window in the group, if any
			hWndCtrl = ::GetWindow(hWndCtrl, GW_HWNDNEXT);
			LRESULT lrButton = SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0L);
			isRadioButton = (hWndCtrl != NULL && (lrButton & DLGC_RADIOBUTTON));

			firstInGroup =  (BOOL)(::GetWindowLongPtr(hWndCtrl, GWL_STYLE) & WS_GROUP);
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_LBString(int nIDC, CString& value)
	//	Perform a data exchange for the state of a list box control on the
	//	DDX/DDV object m_DX with the control numbered nIDC using the given
	//	CString value. On reading the list box, the selected item is returned
	//	in value. If no item is selected, value is set to a string of zero
	//	length. When m_bReadFromControl is false, the item having the given
	//	case-insensitive value as a prefix, if it exists, is selected.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
		{
			  // find the index of the item selected in the list box
			int index = (int)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L);
			if (index != -1)
			{	  // if text was selected, read it into the CString
				int nLen = (int)::SendMessage(hWndCtrl, LB_GETTEXTLEN, index, 0L);
				::SendMessage(hWndCtrl, LB_GETTEXT, index,
					(LPARAM)value.GetBuffer(nLen));

				value.ReleaseBuffer();
			}
			else
			{
				  // no selection, do nothing
				value.Empty();
			}
		}
		else
		{
			  // search the the entire list box for the given value
			  // and select it if it is found
			int index  = ::SendMessage(hWndCtrl, LB_SELECTSTRING,
				(WPARAM)-1, (LPARAM)value.c_str());

			if (index == LB_ERR)
			{
				  // value string was not found
				CString str = CString(_T("Warning: listbox item was not found:  ")) + value + _T(" \n");
				TRACE(str);
			}
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_LBStringExact(int nIDC, CString& value)
	//	This function manages the transfer of CString data between the edit
	//	control of a list box control in a dialog box, form view, or control
	//	view object and a CString data member of that object. It performs a
	//	data exchange for the state of the list box control appearing within
	//	the DDX/DDV object m_DX with the control numbered nIDC. On reading the
	//	list box, the current list box selection is returned as the value.
	//	If no item is selected, value is set to a string of zero length. When
	//	m_DX.m_bReadFromControl is false, the item having the entire given
	//	case-insensitive value as its prefix, if any exists, is selected.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
		{
			  // read and return the CString value
			DDX_LBString(nIDC, value);
		}
		else
		{
			  // find the first entry that matches the entire value,
			  // in a case insensitive search, perhaps in sorted order,
			  // if the box has that style
			int index = (int)::SendMessage(hWndCtrl, LB_FINDSTRINGEXACT,
				(WPARAM)-1, (LPARAM)value.c_str());

			if (index < 0)
			{
				  // no match found
				CString str = (_T("Warning: listbox item was not found:  ")) + value + _T( "\n");
				TRACE(str);
			}
			else
			{
				// select it
				SendMessage(hWndCtrl, LB_SETCURSEL, index, 0L);
			}
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_LBIndex(int nIDC, int& index)
	//	This function manages the transfer of data between a list box
	//	control in a dialog box, form view, or control view object and an
	//	integer index of that object. It performs a data exchange for the
	//	state of the list box control appearing within the DDX/DDV object m_DX
	//	with the control numbered nIDC. When this function is called, index
	//	is set to the index of the current list box selection. If no item
	//	is selected, index is set to LB_ERR. When m_DX.m_bReadFromControl is
	//	false, the index item is selected.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
			index = (int)::SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0L);
		else
			::SendMessage(hWndCtrl, LB_SETCURSEL, (WPARAM)index, 0L);
	}

	//============================================================================
	inline void CDataExchange::DDX_CBString(int nIDC, CString& value)
	//	This function manages the transfer of CString data between the edit
	//	control of a ComboBox control in a dialog box, form view, or control
	//	view object and a CString data member of that object. It performs a
	//	data exchange for the state of the combo box control appearing within
	//	the DDX/DDV object m_DX with the control numbered nIDC. On reading the
	//	list box, the combo box edit window value is returned as the value.
	//	When m_DX.m_bReadFromControl is false, the list of the combo box
	//	is searched for an item that begins with the characters in the value
	//	string. If a matching item is found, it is selected and copied to the
	//	edit control. If the search is unsuccessful, the current edit control
	//	is not changed.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
		{
			  // just get the current edit item text (or drop list static)
			  // (if it works)
			int nLen = ::GetWindowTextLength(hWndCtrl);
			if (nLen > 0)
			{
				  // get the known length
				::GetWindowText(hWndCtrl, value.GetBuffer(nLen), nLen + 1);
			}
			else
			{	  // for drop lists GetWindowTextLength does not work
				  // so here assume max of 255 characters
				const int maxLen = 255;
				::GetWindowText(hWndCtrl, value.GetBuffer(maxLen), maxLen + 1);
			}
			value.ReleaseBuffer();
		}
		else
		{	  // set the current selection based on value string
			if (::SendMessage(hWndCtrl, CB_SELECTSTRING, (WPARAM)-1,
				(LPARAM)value.c_str()) == CB_ERR)
			{
				  // value was not found, so just set the edit text
				  // (this will be ignored if the control is a
				  // DROPDOWNLIST)
				::SetWindowText(hWndCtrl, value);
			}
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_CBStringExact(int nIDC, CString& value)
	//	This function manages the transfer of CString data between the edit
	//	control of a combo box control in a dialog box, form view, or control
	//	view object and a CString data member of that object. It performs a
	//	data exchange for the state of the combo box control appearing within
	//	the DDX/DDV object m_DX with the control numbered nIDC. On reading the
	//	list box, the combo box edit window value is returned as the value.
	//	When m_DX.m_bReadFromControl is false, a case-insensitive search is
	//	made in the prevailing sort sense for the first list box string in
	//	the combo box that matches the string specified in the value parameter.
	//	If a matching item is found, it is selected and copied to the edit
	//	window. If the search is unsuccessful, the current edit control
	//	is not changed.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		if (m_bReadFromControl)
		{
			DDX_CBString(nIDC, value);
		}
		else // write to control
		{
			// set current selection based on data string
			int i = (int)::SendMessage(hWndCtrl, CB_FINDSTRINGEXACT,
				(WPARAM)-1, (LPARAM)value.c_str());
			if (i < 0)
			{
				// just set the edit text
				// (will be ignored if a DROPDOWNLIST)
				::SetWindowText(hWndCtrl, value);
			}
			else
			{
				// select it
				SendMessage(hWndCtrl, CB_SETCURSEL, i, 0L);
			}
		}
	}

	//============================================================================
	inline void CDataExchange::DDX_CBIndex(int nIDC, int& index)
	//	This function manages the transfer of data between a combo box control
	//	in a dialog box, form view, or control view object and an integer
	//	index of that object. It performs a data exchange for the state of
	//	the combo box control appearing within the DDX/DDV object m_DX with
	//	the control numbered nIDC. When this function is called, index is set
	//	to the index of the current combo box selection. If no item is
	//	selected, index is set to 0. When m_DX.m_bReadFromControl is false,
	//	the index item is selected.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);

		if (m_bReadFromControl)
			index = (int)::SendMessage(hWndCtrl, CB_GETCURSEL, 0, 0L);
		else
			::SendMessage(hWndCtrl, CB_SETCURSEL, (WPARAM)index, 0L);
	}

	//============================================================================
	inline void CDataExchange::DDX_Scroll(int nIDC, int& value)
	//	This function manages the transfer of data between a scroll bar control
	//	in a dialog box, form view, or control view object and an integer
	//	value for that object. It performs a data exchange for the state of
	//	the scroll bar control appearing within the DDX/DDV object m_DX with
	//	the control numbered nIDC. When this function is called, value is set
	//	to the current scroll bar position.  When m_DX.m_bReadFromControl
	//	is false, the scroll bar position is set to value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);

		if (m_bReadFromControl)
			value = ::GetScrollPos(hWndCtrl, SB_CTL);
		else
			::SetScrollPos(hWndCtrl, SB_CTL, value, TRUE);
	}

	//============================================================================
	inline void CDataExchange::DDX_Slider(int nIDC, int& value)
	//	This function manages the transfer of data between a slider control
	//	in a dialog box, form view, or control view object and an integer
	//	position value for that object. It performs a data exchange for the
	//	slider control appearing within the DDX/DDV object m_DX with
	//	the control numbered nIDC. When this function is called, value is set
	//	to the current slider entry.  When m_DX.m_bReadFromControl
	//	is false, the slider entry is set to value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);

		if (m_bReadFromControl)
			value = (int) ::SendMessage(hWndCtrl, TBM_GETPOS, 0, 0);
		else
			::SendMessage(hWndCtrl, TBM_SETPOS, TRUE, value);
	}

	//============================================================================
	inline void CDataExchange::DDX_Progress(int nIDC, int& value)
	//	This function manages the transfer of data between a progress control
	//	in a dialog box, form view, or control view object and an integer
	//	progress value for that object. It performs a data exchange for the
	//	slider control appearing within the DDX/DDV object m_DX with
	//	the control numbered nIDC. When this function is called, value is set
	//	to the current slider entry.  When m_DX.m_bReadFromControl
	//	is false, the slider entry is set to value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);

		if (m_bReadFromControl)
			value = (int) ::SendMessage(hWndCtrl, PBM_GETPOS, 0, 0);
		else
			::SendMessage(hWndCtrl, PBM_SETPOS, value, 0);
	}

	//============================================================================
	inline void CDataExchange::DDX_DateTime(int nIDC, SYSTEMTIME &value)
	//	This function manages the transfer of date and/or time data between a
	//	date and time picker control (CDateTime) with control numbered nID
	//	in a dialog box or form view object and a SYSTEMTIME data member of
	//	the object. When called in the read mode, the value is set to the
	//	current state of the control. When called in the write mode, the
	//	current state of the control is set to the given value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		CDateTime* pWndDate = (CDateTime*)CWnd::GetCWndPtr(hWndCtrl);
		assert(pWndDate);

		if (m_bReadFromControl)
			pWndDate->GetTime(&value);
		else
			pWndDate->SetTime(GDT_VALID, &value);
	}

	//============================================================================
	inline void CDataExchange::DDX_MonthCal(int nIDC, SYSTEMTIME& value)
	//	This function manages the transfer of date data between a month
	//	calendar control (CMonthCalendar) with control number nID in a dialog
	//	box, form view, or control view object and a SYSTEMTIME data member
	//	of that object. In particular, the control manages a date value only.
	//	The time fields in the time object are set to reflect the creation
	//	time of the control window, or whatever time was set into the control
	//	with a call to the CMonthCalendar::SetCurSel() member method. In read
	//	mode, value is set to the current state of the month calendar control.
	//	In write mode, the current state is set to the given value.
	{
		HWND hWndCtrl = PrepareCtrl(nIDC);
		CMonthCalendar* pWndMonth = (CMonthCalendar*)CWnd::GetCWndPtr(hWndCtrl);
		assert(pWndMonth);

		if (m_bReadFromControl)
		{
			pWndMonth->GetCurSel(&value);
			value.wHour = 0;
			value.wMinute = 0;
			value.wSecond = 0;
			value.wMilliseconds = 0;
		}
		else
			pWndMonth->SetCurSel(&value);
	}

	////////////////////////////////////////////////////////////////
	//
	//	DDV: Dialog Data Validation Functions
	//
	////////////////////////////////////////////////////////////////

	//============================================================================
	inline void CDataExchange::DDV_MinMaxByte(BYTE value, BYTE minVal, BYTE maxVal)
	//	Check that minVal <= value <= maxVal for the BYTE value and, if not,
	//	show the error box and invoke the m_DX failure procedure.  BYTE is
	//	unsigned char.
	{
		DDV_MinMaxULong((ULONG)value, (ULONG)minVal, (ULONG)maxVal);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxShort(short value, short minVal, short maxVal)
	//	Check that minVal <= value <= maxVal for the short value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		DDV_MinMaxLong((long)value, (long)minVal, (long)maxVal);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxInt(int value, int minVal, int maxVal)
	//	Check that minVal <= value <= maxVal for the int value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		DDV_MinMaxLong((long)value, (long)minVal, (long)maxVal);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxLong(long value, long minVal, long maxVal)
	//	Check that minVal <= value <= maxVal for the long value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		if (!m_allowDDXDDV)
			return;

		assert(minVal <= maxVal);
		if (minVal <= value && value <= maxVal)
			return;

		  // just leave a debugging trace if writing to a control
		if (!m_bReadFromControl)
		{
			TRACE(_T("Warning: current control data is out of range.\n"));
			return;     // don't stop
		}

		  // when reading a number outside the range, put out an error
		  // message with the range tuple
		CString prompt;
		prompt.Format(DDV_MSG_INT_RANGE, minVal, maxVal);
		ErrorMessageBox(prompt);
		Fail();
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxUInt( UINT value, UINT minVal, UINT maxVal)
	//	Check that minVal <= value <= maxVal for the UINT value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		DDV_MinMaxULong((ULONG)value, (ULONG)minVal, (ULONG)maxVal);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxULong(ULONG value, ULONG minVal, ULONG maxVal)
	//	Check that minVal <= value <= maxVal for the ULONG value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		if (!m_allowDDXDDV)
			return;

		assert(minVal <= maxVal);
		if (minVal <= value && value <= maxVal)
			return;

		  // just leave a debugging trace if writing to a control
		if (!m_bReadFromControl)
		{
			int nIDC = ::GetWindowLong(m_hWndLastControl, GWL_ID);
			CString str = CString(_T("Warning: value is outside limits in control with ID "))
						   + nIDC + _T(" \n");
			TRACE(str);
			return;     // don't stop
		}

		  // when reading a number outside the range, put out an error
		  // message with the range tuple
		CString prompt;
		prompt.Format(DDV_MSG_UINT_RANGE, minVal, maxVal);
		ErrorMessageBox(prompt);
		Fail();
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxFloat(float const& value, float minVal,
		float maxVal, int precision /* = FLT_DIG */)
	//	Check that minVal <= value <= maxVal for the float value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		DDV_MinMaxDouble((double)value, (double)minVal, (double)maxVal,
			precision);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxDouble(double const& value, double minVal,
		double maxVal, int precision /* = DBL_DIG */)
	//	Check that minVal <= value <= maxVal for the double value and, if not,
	//	show the error box and invoke the m_DX failure procedure.
	{
		if (!m_allowDDXDDV)
			return;

		assert(minVal <= maxVal);
		if (minVal <= value && value <= maxVal)
			return;

		  // just leave a debugging trace if writing to a control
		if (!m_bReadFromControl)
		{
			TRACE(_T("Warning: current dialog data is out of range.\n"));
			return; // don't Fail()
		}

		  // when reading a number outside the range, put out an error
		  // message with the range tuple
		CString prompt;
		prompt.Format(DDV_MSG_REAL_RANGE, precision, minVal, precision, maxVal);
		ErrorMessageBox(prompt);
		Fail();
	}

	//============================================================================
	inline void CDataExchange::DDV_MaxChars(CString const& value, int nChars)
	//	On reading from the control, check that the length of value <= nChars;
	//	if not, show the error message box and initiate the m_DX failure
	//	procedure. If writing, send the box the message to set the limit
	//	to nChars, which must be at least one.
	{
		if (!m_allowDDXDDV)
			return;

		assert(nChars >= 1);  // must allow at least one char
		if (m_bReadFromControl && value.GetLength() > nChars)
		{
			CString prompt;
			prompt.Format(DDV_MSG_STRING_SIZE, value.c_str(), (long)nChars);
			ErrorMessageBox(prompt);
  			Fail();
		}
		else if (m_hWndLastControl != NULL && m_bEditLastControl)
		{
			  // limit the control max-chars automatically
			::SendMessage(m_hWndLastControl, EM_LIMITTEXT, nChars, 0);
		}
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxSlider(ULONG value, ULONG minVal, ULONG maxVal)
	//	In READFROMCONTROL mode, this method sets the range of the slider
	//	control associated with the last visited window control to the pair
	//	(minRange, maxRange). In SENDTOCONTROL mode, this method verifies
	//	that the refValue first falls between minRange and maxRange values
	//	before setting the range; if refValue is outside these limits,
	//	no setting of the range takes place and a trace message is written
	//	in debug mode.
	{
		if (!m_allowDDXDDV)
			return;

		assert(minVal <= maxVal);
		if (!m_bReadFromControl)
		{
			if (minVal > value || maxVal < value)
			{
	#ifdef _DEBUG
				  // just leave a trace if writing to the control
				int nIDC = ::GetWindowLong(m_hWndLastControl, GWL_ID);
				CString str = CString(_T("Warning: slider position is outside given "))
							+ _T("limits in the control with ID ") + nIDC + _T(" \n");
				TRACE(str);
	#endif
				return;     // don't stop now
			}
		}

		  // set the range tuple
		CSlider* pWndSlider = (CSlider*)CWnd::GetCWndPtr(m_hWndLastControl);
		assert(pWndSlider);

		pWndSlider->SetRangeMin(minVal, FALSE);
		pWndSlider->SetRangeMax(maxVal, TRUE);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxDateTime(SYSTEMTIME& refValue,
		const  SYSTEMTIME& minRange, const  SYSTEMTIME& maxRange)
	//	In READFROMCONTROL mode, this method sets the range of the DateTime
	//	control associated with the last visited window control to the pair
	//	(minRange, maxRange). In SENDTOCONTROL mode, this method verifies
	//	that the refValue first falls between minRange and maxRange values
	//	before setting the range; if refValue is outside these limits,
	//	no setting of the range takes place and a trace message is written
	//	in debug mode.
	{
		if (!m_allowDDXDDV)
			return;

		ULONGLONG zero = (ULONGLONG)0;
		ULONGLONG val = SystemTimeToULL(refValue);
		ULONGLONG min = SystemTimeToULL(minRange);
		ULONGLONG max = SystemTimeToULL(maxRange);
		assert(min == zero || max == zero || min <= max);

		if (!m_bReadFromControl)
		{
			if ((min != zero && min > val) ||
				(max != zero && max < val))
			{
				  // retrieve the control ID
				int nIDC = ::GetWindowLong(m_hWndLastControl, 	GWL_ID);
				CString str = CString(_T("Warning: Date-Time data is out of range "))
								+ _T("in control ID ") + nIDC + _T(" \n");
				TRACE(str);

				return;     // continue on
			}
		}

		  // set the given DateTime range
		SYSTEMTIME sta[2];
		sta[0] = minRange;
		sta[1] = maxRange;
		CDateTime* pWndDate = (CDateTime*)CWnd::GetCWndPtr(m_hWndLastControl);
		assert(pWndDate);

		pWndDate->SetRange(GDTR_MIN | GDTR_MAX, sta);
	}

	//============================================================================
	inline void CDataExchange::DDV_MinMaxMonth(SYSTEMTIME& refValue, const SYSTEMTIME& minRange,
		const SYSTEMTIME& maxRange)
	//	In READFROMCONTROL mode, this method sets the range of the month
	//	calendar control associated with the last visited window control to
	//	(minRange, maxRange). In SENDTOCONTROL mode, this method verifies
	//	that the refValue first falls between minRange and maxRange values
	//	before setting the range; if refValue is outside these limits,
	//	no setting of the range takes place and a trace message is written
	//	in debug mode.
	{
		if (!m_allowDDXDDV)
			return;

		ULONGLONG zero = (ULONGLONG)0;
		ULONGLONG val = SystemTimeToULL(refValue);
		ULONGLONG min = SystemTimeToULL(minRange);
		ULONGLONG max = SystemTimeToULL(maxRange);
		assert(min == zero || max == zero || min <= max);

		if (!m_bReadFromControl)
		{
			if ((min != zero && min > val) ||
				(max != zero && max < val))
			{
				int nIDC = ::GetWindowLong(m_hWndLastControl, GWL_ID);
				CString str = CString(_T("Warning: Calendar data is out of range "))
							+ _T("in control ID ") + nIDC + _T(" \n");
				TRACE(str);
				return;     // continue on
			}
		}

		CMonthCalendar* pWndMonth = (CMonthCalendar*)CWnd::GetCWndPtr(m_hWndLastControl);
		assert(pWndMonth);

		pWndMonth->SetRange(const_cast<LPSYSTEMTIME>(&minRange),
			const_cast<LPSYSTEMTIME>(&maxRange));
	}


	////////////////////////////////////////////////////////////////
	//
	//	DDX/DDV Static Utility Members
	//
	////////////////////////////////////////////////////////////////

	//============================================================================
	inline ULONGLONG SystemTimeToULL(const SYSTEMTIME &systime)
	//	Convert the SYSTEMTIME struct to an ULONGLONG integer and
	//	return this value.
	{
		FILETIME ft;
		SystemTimeToFileTime(&systime, &ft);
		return ((ULONGLONG)(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
	}


}	// namespace Win32xx


#endif // _WIN32XX_DDX_H_

