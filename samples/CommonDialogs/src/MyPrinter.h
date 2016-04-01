/* (09-Feb-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyPrinter.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: The declaration and implementation of the
	MyPrinter and MyPageSetup classes. which are derived from the
	CPrintDialog and CPageSetupDialog classs. These are customized
	extensions that augment the handling of the PrintDlg and PageSetupDlg
	HELP buttons to provide context help and setting of the dialog titles.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
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

********************************************************************************

	Declaration of the MyPrinter/MyPageSetup class

*******************************************************************************/


#ifndef MYPRINTER_H
#define MYPRINTER_H

/******************************************************************************/

#define TF(b) (b ? _T("true") : _T("false"))

/*============================================================================*/
	class
MyPrinter : public CPrintDialog                        			/*

*-----------------------------------------------------------------------------*/
{
	public:
		MyPrinter(
		DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES |
		PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION)
			:	CPrintDialog(dwFlags)
		{
		}

		void SetPDTitle (LPCTSTR title) {m_sPDTitle  = title;}

	protected:
		virtual void OnHelpButton()
		// Send the printer context help identifier to the window whose
		// handle is the object's hwndOwner.
		{
			HWND hwndOwner =  GetParameters().hwndOwner;	
			if (::IsWindow(hwndOwner))
			{
				SendMessage(hwndOwner, WM_COMMAND,
				    IDC_HELP_COMDLG, IDM_HELP_PRINTDLG);
			}
		}

		virtual BOOL OnInitDialog()
		//  Override this member method to perform special processing
		//  when the printer box is initialized. Return TRUE.
		{
			  // do the base class initialization first
			CDialog::OnInitDialog();
			SetWindowTitle();
			return TRUE;
		}

		virtual void OnCancel()
		{
			// for now just show the dialog as cancelled
			::MessageBox(NULL, _T("OnCancel"), _T("Information"),
			MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		}

		virtual void OnOK()
		{
			//  just show the printer information
			int 	copies = GetCopies(),
				from   = GetFromPage(),
				to     = GetToPage();
			CString device = GetDeviceName(),
				driver = GetDriverName(),
				port   = GetPortName(),
				all    = TF(PrintAll()),
				coll   = TF(PrintCollate()),
				range  = TF(PrintRange()),
				select = TF(PrintSelection()),
				rpt;
				rpt.Format(_T("Device : %s\nDriver : %s\n")
				    _T("Port : %s\nRange : %d - %d\n")
				    _T("Copies : %d\n Print all : %s\n")
				    _T("Print collate : %s\nPrint range : %s\n")
				    _T("Print selection : %s\n"),
				    device.c_str(), driver.c_str(),
				    port.c_str(), from, to, copies, all.c_str(),
				    coll.c_str(), range.c_str(), select.c_str());
				::MessageBox(NULL, rpt, _T("Information"),
				    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		}


		virtual void SetWindowTitle() const
		{
			SetWindowText(m_sPDTitle);
		}

	private:
		CString m_sPDTitle;

};

/*============================================================================*/
	class
MyPageSetup : public CPageSetupDialog                            	/*

*-----------------------------------------------------------------------------*/
{
	public:
		MyPageSetup(DWORD dwFlags = PSD_MARGINS)
		    :   CPageSetupDialog(dwFlags)
		{
		}

		void SetPSDTitle(LPCTSTR title) {m_sPSDTitle = title;}

	protected:
		virtual void OnHelpButton()
		//  Send the printer or page setup context help identifier to
		// the window whose handle is the object's hwndOwner,
		//  depending on the dialog box mode
		{
			HWND hwndOwner = GetParameters().hwndOwner;
			if (::IsWindow(hwndOwner))
			{
				SendMessage(hwndOwner, WM_COMMAND,
				    IDC_HELP_COMDLG, (LPARAM)IDM_HELP_PAGESETDLG);
			}
		}

		virtual BOOL OnInitDialog()
		//  Override this member method to perform special processing
		//  when the printer box is initialized. Return TRUE.
		{
			  // do the base class initialization first
			CDialog::OnInitDialog();
			SetWindowTitle();
			return TRUE;
		}

		virtual void OnCancel()
		{
			// for now just show the dialog as cancelled
			::MessageBox(NULL, _T("OnCancel"), _T("Information"),
			MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		}

		virtual void OnOK()
		{
			CSize papersize = GetPaperSize();
			CRect margins, minMargins;
			GetMargins(margins, minMargins);
			DWORD flags = GetParameters().Flags;
			CDevMode lpDev = GetDevMode();	
			LPCTSTR orient = (lpDev.Get() ?
			    (lpDev->dmOrientation == DMORIENT_PORTRAIT  ?
			    _T("portrait") : _T("landscape")) : _T("ERROR!!!"));
			LPCTSTR units = (flags & PSD_INHUNDREDTHSOFMILLIMETERS ?
			    _T("1/100 mm") : (flags & PSD_INTHOUSANDTHSOFINCHES ?
			    _T("1/1000 in") : _T("unknown")));
			CString rpt;
			rpt.Format(_T("paper size (%d, %d)\n")
			    _T("margins (%d, %d, %d, %d)\n")
			    _T("min margins (%d, %d, %d, %d)\n")
			    _T("units: %s\norientation: %s"),
			    papersize.cx, papersize.cy,
			    margins.left, margins.top, margins.right,
			    margins.bottom, minMargins.left, minMargins.top,
			    minMargins.right, minMargins.bottom, units, orient);
			::MessageBox(NULL, rpt, _T("Information"),
			    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		}

		virtual void SetWindowTitle() const
		{
			SetWindowText( m_sPSDTitle);
		}

	private:
		CString  m_sPSDTitle;

};
/*----------------------------------------------------------------------------*/
#endif  // MYPRINTER_H


