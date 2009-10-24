#include "statusbar.h"


namespace Win32xx
{

	//////////////////////////////////////
	// Definitions for the CStatusbar class
	//
	CStatusbar::CStatusbar()
	{
	}

	void CStatusbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM | SBARS_SIZEGRIP;
		cs.lpszClass = STATUSCLASSNAME;
	}

	void CStatusbar::CreateParts(int iParts, const int iPaneWidths[]) const
	{
		// If an element of iPaneWidths is -1, the right edge of the corresponding part extends
		//  to the border of the window
		if (IsWindow())
		{
			if (iParts > 256)
				throw CWinException (_T("CStatusbar::CreateParts ... Too many panes"));

			// Create the statusbar panes
			if (!SendMessage(SB_SETPARTS, iParts, (LPARAM)iPaneWidths))
				throw CWinException(_T("CStatusbar::CreateParts failed"));
		}
	}

	int CStatusbar::GetParts()
	{
		return (int)SendMessage(SB_GETPARTS, 0L, 0L);
	}

	HICON CStatusbar::GetPartIcon(int iPart)
	{
		return (HICON)SendMessage(SB_GETICON, (WPARAM)iPart, 0L);
	}

	CRect CStatusbar::GetPartRect(int iPart)
	{
		CRect rc;
		SendMessage(SB_GETRECT, (WPARAM)iPart, (LPARAM)&rc);
		return rc;
	}

	tString CStatusbar::GetPartText(int iPart) const
	{
		tString PaneText;
		try
		{
			if (IsWindow())
			{
				// Get size of Text array
				int iChars = LOWORD (SendMessage(SB_GETTEXTLENGTH, iPart, 0L));

				// Get the Text
				TCHAR* szText = new TCHAR[iChars +1 ];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == szText)
					throw std::bad_alloc();

				szText[0] = _T('\0');
				SendMessage(SB_GETTEXT, iPart, (LPARAM)szText);

				//Store the text in the member variable
				PaneText = szText;
				delete []szText;
			}
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Failed to allocate memory in CStatusbar::GetPartText"));
			PaneText = _T("");
			// Not a critical problem, so no need to rethrow
		}

		return PaneText;
	}

	BOOL CStatusbar::IsSimple()
	{
		return (BOOL)SendMessage(SB_ISSIMPLE, 0L, 0L);
	}

	void CStatusbar::SetPartText(int iPart, LPCTSTR szText, UINT Style) const
	// Available Styles: Combinations of ...
	//0					The text is drawn with a border to appear lower than the plane of the window.
	//SBT_NOBORDERS		The text is drawn without borders.
	//SBT_OWNERDRAW		The text is drawn by the parent window.
	//SBT_POPOUT		The text is drawn with a border to appear higher than the plane of the window.
	//SBT_RTLREADING	The text will be displayed in the opposite direction to the text in the parent window.
	{
		if (IsWindow())
		{
			if (SendMessage(SB_GETPARTS, 0L, 0L) >= iPart)
			{
				if (!SendMessage(SB_SETTEXT, iPart | Style, (LPARAM)szText))
					throw CWinException(_T("Failed to set status bar text"));
			}
		}
	}

	BOOL CStatusbar::SetPartIcon(int iPart, HICON hIcon)
	{
		return (BOOL)SendMessage(SB_SETICON, (WPARAM)iPart, (LPARAM) hIcon);
	}

	void CStatusbar::SetPartWidth(int iPart, int iWidth) const
	{
		// This changes the width of an existing pane, or creates a new pane
		// with the specified width

		int* iPartWidths = NULL;
		int* iNewPartWidths = NULL;

		try
		{
			if (IsWindow())
			{
				if ((iPart > 256) || (iWidth < 0))
					throw CWinException (_T("CStatusbar::SetPartWidth ... Invalid parameters"));

				if (iPart < 0) iPart = 0;

				int iParts = (int)SendMessage(SB_GETPARTS, 0L, 0L);
				iPartWidths = new int[iParts];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == iPartWidths)
					throw std::bad_alloc();

				SendMessage(SB_GETPARTS, iParts, (LPARAM)iPartWidths);

				int iNewParts = MAX(iPart+1, iParts);
				iNewPartWidths = new int[iNewParts];
				if (NULL == iNewPartWidths)
					throw std::bad_alloc();

				ZeroMemory(iNewPartWidths, iNewParts*sizeof(int));

				for (int i = 0; i < iParts; ++i)
					iNewPartWidths[i] = iPartWidths[i];

				if (0 == iPart)
					iNewPartWidths[iPart] = iWidth;
				else
					iNewPartWidths[iPart] = iNewPartWidths[iPart -1] + iWidth;

				if (!SendMessage(SB_SETPARTS, iNewParts, (LPARAM)iNewPartWidths))
					throw CWinException(_T("CStatusbar::SetPartWidth failed"));

				delete []iNewPartWidths;
				delete []iPartWidths;
			}
		}

		catch (const CWinException &e)
		{
			if (iNewPartWidths) delete []iNewPartWidths;
			if (iPartWidths)	delete []iPartWidths;

			e.MessageBox();
		}

		catch (const std::bad_alloc &)
		{
			if (iNewPartWidths) delete []iNewPartWidths;
			if (iPartWidths)	delete []iPartWidths;

			DebugErrMsg(_T("Exception in CStatusbar::SetPartWidth"));
			// Not a critical problem, so no need to rethrow
		}
	}

	void CStatusbar::SetSimple(BOOL fSimple /* = TRUE*/)
	{
		SendMessage(SB_SIMPLE, (WPARAM)fSimple, 0L);
	}

} // namespace Win32xx
