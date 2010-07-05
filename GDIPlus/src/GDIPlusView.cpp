//////////////////////////////////////////////
// GDIPlusView.cpp
//  Definitions for the CGDIPlusView class

#include "stdafx.h"
#include "GDIPlusView.h"
using namespace Gdiplus;

CGDIPlusView::CGDIPlusView()
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CGDIPlusView::~CGDIPlusView()
{
	// Shutdown GDI+
	GdiplusShutdown(m_gdiplusToken);
}

void CGDIPlusView::DrawCappedLine(HDC hdc) 
{
	Graphics graphics(hdc);
	// Draw capped line, width 8
	Pen penCapped(Color(255, 0, 0, 255), 8);
	Status stat = penCapped.SetStartCap(LineCapArrowAnchor);
	stat = penCapped.SetEndCap(LineCapRoundAnchor);
	stat = graphics.DrawLine(&penCapped, 10, 175, 300, 175);
}

void CGDIPlusView::DrawGamaShapes(HDC hdc)
{
	Graphics graphics(hdc);

	// Draw Plygons with Gama Corrections
	// Put the points of a polygon in an array.
	GraphicsPath pathGama;
	int yOffset = 200;
	Point points[] = {Point(75,  0  +yOffset), Point(100, 50+yOffset), 
					  Point(150, 50 +yOffset), Point(112, 75+yOffset),
					  Point(150, 150+yOffset), Point(75,  100+yOffset), 
					  Point(0,   150+yOffset), Point(37,  75+yOffset), 
					  Point(0,   50 +yOffset), Point(50,  50+yOffset)};

	// Use the array of points to construct a path.
	pathGama.AddLines(points, 10);

	// Use the path to construct a path gradient brush.
	PathGradientBrush pthGrBrushGama(&pathGama);

	// Set the color at the center of the path to red.
	pthGrBrushGama.SetCenterColor(Color(255, 255, 0, 0));

	// Set the colors of the points in the array.
	Color colorsGama[] = {Color(255, 0, 0, 0),   Color(255, 0, 255, 0),
					  Color(255, 0, 0, 255), Color(255, 255, 255, 255), 
					  Color(255, 0, 0, 0),   Color(255, 0, 255, 0), 
					  Color(255, 0, 0, 255), Color(255, 255, 255, 255),
					  Color(255, 0, 0, 0),   Color(255, 0, 255, 0)};

	int count = 10;
	pthGrBrushGama.SetSurroundColors(colorsGama, &count);

	// Fill the path with the path gradient brush.
	graphics.FillPath(&pthGrBrushGama, &pathGama);
	pthGrBrushGama.SetGammaCorrection(TRUE);
	graphics.TranslateTransform(200.0f, 0.0f);
	graphics.FillPath(&pthGrBrushGama, &pathGama);
}

void CGDIPlusView::DrawGradientElipse(HDC hdc)
{
	Graphics graphics(hdc);

	// Create a path that consists of a single ellipse.
	GraphicsPath path;
	path.AddEllipse(0, 80, 140, 70);

	// Use the path to construct a brush.
	PathGradientBrush pthGrBrush(&path);

	// Set the color at the center of the path to blue.
	pthGrBrush.SetCenterColor(Color(255, 0, 0, 255));

	// Set the color along the entire boundary of the path to aqua.
	Color colors[] = {Color(255, 0, 255, 255)};
	int count = 1;
	pthGrBrush.SetSurroundColors(colors, &count);

	graphics.FillEllipse(&pthGrBrush, 0, 80, 140, 70);
}

void CGDIPlusView::DrawSolidElipse(HDC hdc)
{
	Graphics graphics(hdc);

	SolidBrush solidBrush(Color(255, 255, 0, 0));
	graphics.FillEllipse(&solidBrush, 160, 84, 100, 60);
}

void CGDIPlusView::DrawSolidLine(HDC hdc) 
{
	Graphics graphics(hdc);
	
	// Draw solid line
	Pen      penLine(Color(255, 0, 0, 255));
	graphics.DrawLine(&penLine, 10, 70, 200, 70);
}

void CGDIPlusView::DrawText(HDC hdc) 
{
	Graphics graphics(hdc);

	// Draw some text
	SolidBrush  brush(Color(255, 0, 0, 255));
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(10.0f, 20.0f);
   
	graphics.DrawString(L"GDI+  Example", -1, &font, pointF, &brush);
}

void CGDIPlusView::OnInitialUpdate()
{
	// OnInitialUpdate is called immediately after the window is created
	TRACE(_T("View window created\n"));
}

void CGDIPlusView::OnPaint(HDC hDC)
{
	DrawSolidLine(hDC);
	DrawText(hDC);
	DrawCappedLine(hDC);
	DrawGradientElipse(hDC);
	DrawSolidElipse(hDC);
	DrawGamaShapes(hDC); 
}

void CGDIPlusView::PreCreate(CREATESTRUCT &cs)
{
	// Here we set the defaults used by the create function for the view window
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the window we create.

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CGDIPlusView::RegisterClass(WNDCLASS &wc)
{
	// Here we set the Window class parameters.
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the type of window we create.

	// Set the Window Class name
	wc.lpszClassName = _T("View");

	// Set the class style (not to be confused with the window styles set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}

LRESULT CGDIPlusView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

