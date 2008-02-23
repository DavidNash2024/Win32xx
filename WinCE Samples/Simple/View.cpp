#include "view.h"
#include "Resource.h"


void CView::OnPaint(HDC hDC)
{
	// Define a rectangle to contain some text
	RECT r;
	::GetClientRect(m_hWnd, &r);
	r.bottom = r.bottom/2;

	// Draw some text in our rectangle
	LPCTSTR szString = TEXT("Use Debug mode. Click here");
	DrawText(hDC, szString, lstrlen(szString), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}


LRESULT CView::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
	 		//Post the WM_QUIT message to terminate the application.
			::PostQuitMessage(0);
			return 0;
		case WM_LBUTTONDOWN:
			// NOTE: You must be in debug mode to see the trace output
			TRACE(TEXT("Left mouse button down message\n"));
			break;
		case WM_LBUTTONUP:
			// NOTE: You must be in debug mode to see the trace output
			TRACE(TEXT("Left mouse button up message\n"));
			break;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(hwnd, uMsg, wParam, lParam);
}

