///////////////////////////////////////
// MetaView.cpp

#include "MetaView.h"
#include "resource.h"

int CMetaView::OnCreate(CREATESTRUCT&)
{
	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

	// Set the window title
	SetWindowText(LoadString(IDW_MAIN));


	CMetaFileDC dcMeta;
	// Creates an enhanced MetaFile called "Pattern.emf", and also the MetaFile device context
	dcMeta.CreateEnhanced(NULL, _T("Pattern.emf"), NULL, NULL);
	
	// Draw to the Metafile device context
	dcMeta.Rectangle(0, 0, 100, 100);
	dcMeta.MoveTo(0, 0);
	dcMeta.LineTo(100, 100);
	dcMeta.MoveTo(0, 100);
	dcMeta.LineTo(100, 0);
	dcMeta.CreateSolidBrush(RGB(0, 255, 0));
	dcMeta.Ellipse(20, 20, 80, 80);

	// Close the metafile. The CEnhMetaFile is now ready for use.
	m_EnhMetaFile = dcMeta.CloseEnhanced();

	return 0;
}

void CMetaView::OnDraw(CDC& dc)
{
	CRect rcClient = GetClientRect();
	int cxClient = rcClient.Width();
	int cyClient = rcClient.Height();
	SetMapMode(dc, MM_ANISOTROPIC);
	SetWindowExtEx(dc, 1000, 1000, NULL);
	SetViewportExtEx(dc, cxClient, cyClient, NULL);

	CRect rc(0, 0, 100, 100);

	// Display 10x10 images of the metafile in the window
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			dc.SetWindowOrgEx(-100 * x, -100 * y, NULL);		
			dc.PlayMetaFile(m_EnhMetaFile, rc);
		}
	}
}

void CMetaView::OnDestroy() 
{
	PostQuitMessage(0);
}

LRESULT CMetaView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		RedrawWindow();
		return 0;
	}
		
	return WndProcDefault(uMsg, wParam, lParam);
}

