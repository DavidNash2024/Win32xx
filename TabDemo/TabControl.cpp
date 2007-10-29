#include "../Win32++/Wincore.h"
#include "resource.h"
#include "TabControl.h"


HWND CTabCtrl::Create(HWND hwndParent)
{
	RECT rcClient;
	TCITEM tie;
	int i;
	TCHAR szText[256];

	// Get the dimensions of the parent window's client area, and
	// create a tab control child window of that size.
	GetClientRect(hwndParent, &rcClient);
	InitCommonControls();
	CreateEx(0, WC_TABCONTROL, "", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
				0, 0, rcClient.right, rcClient.bottom, hwndParent, NULL, NULL);

	// Add tabs for each day of the week.
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = szText;

	for (i = 0; i < 7; i++) 
	{
		lstrcpy(szText, LoadString(IDS_FIRSTDAY + i));
		if (TabCtrl_InsertItem(m_hWnd, i, &tie) == -1) 
		{
			DestroyWindow();
			return NULL;
		}
	}
	
	return m_hWnd;
}

