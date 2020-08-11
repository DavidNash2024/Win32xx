/////////////////////////////
// GDIPlusView.cpp
//

#include "stdafx.h"
#include "GDIPlusView.h"

using namespace Gdiplus;

////////////////////////////////////
// CGDIPlusView function definitions
//

// Constructor.
CGDIPlusView::CGDIPlusView()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

// Destructor.
CGDIPlusView::~CGDIPlusView()
{
    // Shutdown GDI+
    GdiplusShutdown(m_gdiplusToken);
}

// Draws a capped line.
void CGDIPlusView::DrawCappedLine(CDC& dc)
{
    Graphics graphics(dc);
    // Draw capped line, width 8
    Pen penCapped(Color(255, 0, 0, 255), 8);
    penCapped.SetStartCap(LineCapArrowAnchor);
    penCapped.SetEndCap(LineCapRoundAnchor);
    graphics.DrawLine(&penCapped, 10, 175, 300, 175);
}

// Draws some polygons with gama corrections.
void CGDIPlusView::DrawGamaShapes(CDC& dc)
{
    Graphics graphics(dc);

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

// Draw an elipse with a gradient brush.
void CGDIPlusView::DrawGradientElipse(CDC& dc)
{
    Graphics graphics(dc);

    // Create a path that consists of a single ellipse.
    GraphicsPath path;
    path.AddEllipse(0, 80, 140, 70);

    // Use the path to construct a brush.
    PathGradientBrush pathBrush(&path);

    // Set the color at the center of the path to blue.
    pathBrush.SetCenterColor(Color(255, 0, 0, 255));

    // Set the color along the entire boundary of the path to aqua.
    Color colors[] = {Color(255, 0, 255, 255)};
    int count = 1;
    pathBrush.SetSurroundColors(colors, &count);

    graphics.FillEllipse(&pathBrush, 0, 80, 140, 70);
}

// Draw a solid elipse.
void CGDIPlusView::DrawSolidElipse(CDC& dc)
{
    Graphics graphics(dc);

    SolidBrush solidBrush(Color(255, 255, 0, 0));
    graphics.FillEllipse(&solidBrush, 160, 84, 100, 60);
}

// Draw a solid line.
void CGDIPlusView::DrawSolidLine(CDC& dc)
{
    Graphics graphics(dc);

    // Draw solid line
    Pen penLine(Color(255, 0, 0, 255));
    graphics.DrawLine(&penLine, 10, 70, 200, 70);
}

// Draw some text.
void CGDIPlusView::DrawText(CDC& dc)
{
    Graphics graphics(dc);

    // Draw some text
    SolidBrush  brush(Color(255, 0, 0, 255));
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF      pointF(10.0f, 20.0f);

    graphics.DrawString(L"GDI+  Example", -1, &font, pointF, &brush);
}

// Called when part of the window needs to be redrawn.
void CGDIPlusView::OnDraw(CDC& dc)
{
    DrawSolidLine(dc);
    DrawText(dc);
    DrawCappedLine(dc);
    DrawGradientElipse(dc);
    DrawSolidElipse(dc);
    DrawGamaShapes(dc);
}

// Called after the window is created.
void CGDIPlusView::OnInitialUpdate()
{
    // OnInitialUpdate is called immediately after the window is created
    TRACE("View window created\n");
}

// Set the CREATESTRUCT parameters before the window is created.
void CGDIPlusView::PreCreate(CREATESTRUCT& cs)
{
    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

// Set the WNDCLASS parameters before the window is created.
void CGDIPlusView::RegisterClass(WNDCLASS& wc)
{
    // Set the Window Class name
    wc.lpszClassName = _T("View");

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

// Process the window messages for the view window.
LRESULT CGDIPlusView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

