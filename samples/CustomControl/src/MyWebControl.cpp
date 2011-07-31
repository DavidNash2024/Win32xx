#include "stdafx.h"
#include "MyWebControl.h"

CWebControl::CWebControl()
{
	// The entry for the dialog's control in resource.rc must match this name.
	CString ClassName = _T("WebControl");

	// Register the window class
	WNDCLASS wc = {0};

	if (!::GetClassInfo(GetApp()->GetInstanceHandle(), ClassName, &wc))
	{
		wc.lpszClassName = ClassName;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.hInstance = GetApp()->GetInstanceHandle();
		::RegisterClass(&wc);
	}

	assert (::GetClassInfo(GetApp()->GetInstanceHandle(), ClassName, &wc));
}

void CWebControl::OnInitialUpdate()
{
	// Go to the browser's home page
	GetIWebBrowser2()->GoHome();
}



