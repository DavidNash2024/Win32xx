//////////////////////////////////////////////
// MyProgressbar.cpp

#include "stdafx.h"
#include "Progressbar.h"
#include "DialogApp.h"

void CMyProgressbar::SetProgress(int nPos)
{
	// Set the progress bar position
	::SendMessage(m_hWnd, PBM_SETPOS, (WPARAM)nPos, 0);
}

