//////////////////////////////////////////////
// MySlider.cpp

#include "stdafx.h"
#include "Slider.h"
#include "DialogApp.h"

void CMySlider::OnInitDialog()
{
	// Set a tic mark every ten units for the slider control
	::SendMessage(m_hWnd, TBM_SETTICFREQ,  (WPARAM)10, 0);
}

LRESULT CMySlider::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
	case WM_HSCROLL:
		{
			// Get the slider bar position
			int nPos = SendMessage(TBM_GETPOS, 0, 0);

			// Get a reference to the MyDialog object
			CMyDialog& MyDialog = GetDialogApp().GetDialog();

			MyDialog.SetProgress(nPos);		// Set the progress bar position
			MyDialog.SetScroll(nPos);		// Set the scroll bar position
			MyDialog.SetStatic(TRUE, nPos);	// Set the static text
			break;
		}
	}

	return 0;
}

void CMySlider::SetSlider(int nPos)
{
	// Set the slider position
	::SendMessage(m_hWnd, TBM_SETPOS, TRUE, nPos);
}

