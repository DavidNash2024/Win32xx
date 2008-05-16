#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "WinCore.h"

namespace Win32xx
{

	//////////////////////////////////////
	// Declaration of the CStatusbar class
	//
	class CStatusbar : public CWnd
	{
	public:
		CStatusbar();
		virtual ~CStatusbar() {}
		virtual void CreatePanes(int iPanes, const int iPaneWidths[]) const;
		virtual tString GetPaneText(int iPane);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetPaneText(int iPane, LPCTSTR szText, UINT Style = 0) const;
		virtual void SetPaneWidth(int iPane, int iWidth) const;

	};

	//////////////////////////////////////
	// Definitions for the CStatusbar class
	//
	inline CStatusbar::CStatusbar()
	{
	}

	inline void CStatusbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM | SBARS_SIZEGRIP;
		cs.lpszClass = STATUSCLASSNAME;
	}

	inline void CStatusbar::CreatePanes(int iPanes, const int iPaneWidths[]) const
	{
		// If an element of iPaneWidths is -1, the right edge of the corresponding part extends
		//  to the border of the window
		if (::IsWindow(m_hWnd))
		{
			if (iPanes > 256)
				throw CWinException (_T("CStatusbar::CreatePanes ... Too many panes"));

			// Create the statusbar panes
			if (!::SendMessage(m_hWnd, SB_SETPARTS, iPanes, (LPARAM)iPaneWidths))
				throw CWinException(_T("CStatusbar::CreatePanes failed"));
		}
	}

	inline tString CStatusbar::GetPaneText(int iPane)
	{
		tString PaneText;
		try
		{
			if (::IsWindow(m_hWnd))
			{
				// Get size of Text array
				int iChars = LOWORD (::SendMessage(m_hWnd, SB_GETTEXTLENGTH, iPane, 0));

				// Get the Text
				TCHAR* szText = new TCHAR[iChars +1 ];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == szText)
					throw std::bad_alloc();

				szText[0] = _T('\0');
				::SendMessage(m_hWnd, SB_GETTEXT, iPane, (LPARAM)szText);

				//Store the text in the member variable
				PaneText = szText;
				delete []szText;
			}
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Failed to allocate memory in CStatusbar::GetPaneText"));
			PaneText = _T("");
			// Not a critical problem, so no need to rethrow
		}

		return PaneText;
	}

	inline void CStatusbar::SetPaneText(int iPane, LPCTSTR szText, UINT Style) const
	// Available Styles: Combinations of ...
	//0					The text is drawn with a border to appear lower than the plane of the window.
	//SBT_NOBORDERS		The text is drawn without borders.
	//SBT_OWNERDRAW		The text is drawn by the parent window.
	//SBT_POPOUT		The text is drawn with a border to appear higher than the plane of the window.
	//SBT_RTLREADING	The text will be displayed in the opposite direction to the text in the parent window.
	{
		if (::IsWindow(m_hWnd))
		{
			if (::SendMessage(m_hWnd, SB_GETPARTS, 0, 0) >= iPane)
			{
				if (!::SendMessage(m_hWnd, SB_SETTEXT, iPane | Style, (LPARAM)szText))
					throw CWinException(_T("Failed to set status bar text"));
			}
		}
	}

	inline void CStatusbar::SetPaneWidth(int iPane, int iWidth) const
	{
		// This changes the width of an existing pane, or creates a new pane
		// with the specified width

		int* iPaneWidths = NULL;
		int* iNewPaneWidths = NULL;

		try
		{
			if (::IsWindow(m_hWnd))
			{
				if ((iPane > 256) || (iWidth < 0))
					throw CWinException (_T("CStatusbar::SetPaneWidth ... Invalid parameters"));

				if (iPane < 0) iPane = 0;

				int iParts = (int)::SendMessage(m_hWnd, SB_GETPARTS, 0, 0);
				iPaneWidths = new int[iParts];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == iPaneWidths)
					throw std::bad_alloc();

				::SendMessage(m_hWnd, SB_GETPARTS, iParts, (LPARAM)iPaneWidths);

				int iNewParts = max(iPane+1, iParts);
				iNewPaneWidths = new int[iNewParts];
				if (NULL == iNewPaneWidths)
					throw std::bad_alloc();

				ZeroMemory(iNewPaneWidths, iNewParts*sizeof(int));

				for (int i = 0; i < iParts; ++i)
					iNewPaneWidths[i] = iPaneWidths[i];

				if (0 == iPane)
					iNewPaneWidths[iPane] = iWidth;
				else
					iNewPaneWidths[iPane] = iNewPaneWidths[iPane -1] + iWidth;

				if (!::SendMessage(m_hWnd, SB_SETPARTS, iNewParts, (LPARAM)iNewPaneWidths))
					throw CWinException(_T("CStatusbar::SetPaneWidth failed"));

				delete []iNewPaneWidths;
				delete []iPaneWidths;
			}
		}

		catch (const CWinException &e)
		{
			if (iNewPaneWidths) delete []iNewPaneWidths;
			if (iPaneWidths)	delete []iPaneWidths;

			e.MessageBox();
		}

		catch (const std::bad_alloc &)
		{
			if (iNewPaneWidths) delete []iNewPaneWidths;
			if (iPaneWidths)	delete []iPaneWidths;

			DebugErrMsg(_T("Exception in CStatusbar::SetPaneWidth"));
			// Not a critical problem, so no need to rethrow
		}
	}

} // namespace Win32xx

#endif // #ifndef STATUSBAR_H
