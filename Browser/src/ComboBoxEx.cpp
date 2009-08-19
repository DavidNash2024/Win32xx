#include "ComboBoxEx.h"


void CComboBoxEx::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = _T("COMBOBOXEX32");
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}

BOOL CComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	// translate keyboard input for the edit control
	if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
	{
		if (IsDialogMessage(m_hWnd, pMsg))
			return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

