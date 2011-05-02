//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "GDI.h"
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
	CDC dc = GetDC();

	dc.CreatePen(PS_SOLID, 1, m_points.back().color);
	dc.MoveTo(m_points.back().x, m_points.back().y);
	dc.LineTo(x, y);
}

void CView::OnPaint(CDC& dc)
{
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			dc.CreatePen(PS_SOLID, 1, m_points[i].color);
			if (bDraw)
				dc.LineTo(m_points[i].x, m_points[i].y);
			else
				dc.MoveTo(m_points[i].x, m_points[i].y);
			
			bDraw = m_points[i].PenDown;
		}
	}
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

void CView::FileOpen(LPCTSTR szFilename)
{
	// empty the PlotPoint vector
	m_points.clear();

	DWORD nBytesRead;

	// Create a handle to the file
	HANDLE hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		do
		{
			nBytesRead = 0;
			PlotPoint pp;

			// Read a PlotPoint struct from the file
			if (!ReadFile(hFile, &pp, sizeof(PlotPoint), &nBytesRead, NULL))
				throw CWinException(_T("Failed to read from file"));

			// store the PlotPoint in the PlotPoint vector
			if (nBytesRead == sizeof(PlotPoint))
				m_points.push_back(pp);
			else if (nBytesRead != 0)
			{
				m_points.clear();
				throw CWinException (_T("Error while reading from file"));
			}


		} while (nBytesRead == sizeof(PlotPoint));

		CloseHandle(hFile);
	}
	else
		throw CWinException(_T("Failed to open file for reading"));

	// repaint the view window
	Invalidate();
}


void CView::FileSave(LPCTSTR szFilename)
{
	DWORD nBytesWritten;
	HANDLE hFile = CreateFile(szFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE != hFile)
	{
		for (int i = 0; i < (int)m_points.size(); ++i)
		{
			if ((!WriteFile(hFile, &m_points[i], sizeof(PlotPoint), &nBytesWritten, NULL))
				|| (nBytesWritten != sizeof(PlotPoint)))
			{
				throw CWinException (_T("Error while writing to file"));
			}
		}

		CloseHandle(hFile);
	}
	else
		throw CWinException(_T("Failed to open file for writing"));
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
	if ( (wParam & MK_LBUTTON) && (GetCapture() == this) )
	{
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


