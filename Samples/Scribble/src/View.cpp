//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"


using namespace std;

CView::CView() : m_PenColor(RGB(0,0,0))
{
	m_Brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::ClearPoints()
{
	m_points.clear();
	Invalidate();
}

void CView::DrawLine(int x, int y)
{
	CDC DrawDC = GetDC();
	DrawDC.CreatePen(PS_SOLID, 1, m_points.back().color);
	DrawDC.MoveTo(m_points.back().x, m_points.back().y);
	DrawDC.LineTo(x, y);
}

void CView::OnPaint(CDC& dc)
{
	
	// Here we use double buffering (drawing to a memory DC) for smoother rendering
	// Set up our Memory DC and bitmap
	CDC MemDC = dc.CreateCompatibleDC();
	int Width = GetClientRect().Width();
	int Height = GetClientRect().Height();
	MemDC.CreateCompatibleBitmap(dc, Width, Height);
	MemDC.FillRect(GetClientRect(), m_Brush);	

	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
		
			MemDC.CreatePen(PS_SOLID, 1, m_points[i].color);
			if (bDraw)
				MemDC.LineTo(m_points[i].x, m_points[i].y);
			else 
				MemDC.MoveTo(m_points[i].x, m_points[i].y);
			
			bDraw = m_points[i].PenDown;			
		}
	}

	// Copy from the memory DC to our painting dc
	dc.BitBlt(0, 0, Width, Height, MemDC, 0, 0, SRCCOPY);
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the extra style to provide a sunken effect
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Set the background brush, class name and cursor
	wc.hbrBackground = m_Brush;
	wc.lpszClassName = _T("Scribble Window");
	wc.hCursor = ::LoadCursor(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1));
}

BOOL CView::FileOpen(LPCTSTR szFilename)
{
	// empty the PlotPoint vector
	m_points.clear();
	DWORD nBytesRead;
	BOOL bResult = FALSE;

	// Create a handle to the file
	HANDLE hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		do
		{
			nBytesRead = 0;
			PlotPoint pp;

			if (ReadFile(hFile, &pp, sizeof(PlotPoint), &nBytesRead, NULL))
			{
				if (nBytesRead == sizeof(PlotPoint))
					m_points.push_back(pp);
			}
			else
			{
				m_points.clear();
				::MessageBox (0, _T("Failed to read from file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
				break;
			}

		} while (nBytesRead == sizeof(PlotPoint));

		if ((0 != nBytesRead) || (m_points.empty()))
		{
			// Failed to read all of the file
			m_points.clear();
			::MessageBox (0, _T("Invalid data in file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		}
		else
			bResult = TRUE;

		CloseHandle(hFile);
	}
	else
	{
		tString tsErrMsg = _T("Failed to open file ");
		tsErrMsg += szFilename;
		::MessageBox (0, tsErrMsg.c_str(), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
	}

	Invalidate();
	return bResult;
}

BOOL CView::FileSave(LPCTSTR szFilename)
{
	DWORD nBytesWritten;
	HANDLE hFile = CreateFile(szFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		::MessageBox (0, _T("Failed to open file for writing"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	BOOL bResult = TRUE;
	for (size_t i = 0; i < m_points.size(); ++i)
	{
		if ((!WriteFile(hFile, &m_points[i], sizeof(PlotPoint), &nBytesWritten, NULL))
			|| (nBytesWritten != sizeof(PlotPoint)))
		{
			::MessageBox (0, _T("Error while writing to file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
			bResult = FALSE;
			break;
		}
	}

	CloseHandle(hFile);
	return bResult;
}

void CView::SetPen(COLORREF color)
{
	m_PenColor = color;
}

void CView::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;
	P1.color = m_PenColor;

	m_points.push_back(P1); //Add the point to the vector
}

void CView::OnLButtonDown(LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
}

void CView::OnLButtonUp(LPARAM lParam)
{
	{
		//Release the capture on the mouse
		ReleaseCapture();

		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
	}
}

void CView::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if (wParam & MK_LBUTTON)
	{
		TCHAR str[80];
		::wsprintf(str, TEXT("Draw Point:  %hd, %hd\n"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		TRACE(str);

		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
	}
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown(lParam);
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
        break;

    case WM_LBUTTONUP:
		OnLButtonUp(lParam);
		break;
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

