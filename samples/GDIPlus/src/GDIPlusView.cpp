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
    int scale = DPIScaleInt(1);
    graphics.DrawLine(&penCapped, 10 * scale, 175 * scale, 300 * scale, 175 * scale);
}

// Draws some polygons with gama corrections.
void CGDIPlusView::DrawGamaShapes(CDC& dc)
{
    Graphics graphics(dc);

    // Put the points of a polygon in an array.
    GraphicsPath pathGama;
    int scale = DPIScaleInt(1);
    int yOffset = 200 * scale;
    Point points[] = {Point(75 * scale,  0  + yOffset),
                      Point(100 * scale, 50 * scale + yOffset),
                      Point(150 * scale, 50 * scale + yOffset),
                      Point(112 * scale, 75 * scale + yOffset),
                      Point(150 * scale, 150 * scale + yOffset),
                      Point(75 * scale,  100 * scale + yOffset),
                      Point(0,           150 * scale + yOffset),
                      Point(37 * scale,  75 * scale + yOffset),
                      Point(0,           50 * scale + yOffset),
                      Point(50 * scale,  50 * scale + yOffset)};

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
    graphics.TranslateTransform(200.0f * scale, 0.0f);
    graphics.FillPath(&pthGrBrushGama, &pathGama);
}

// Draw an ellipse with a gradient brush.
void CGDIPlusView::DrawGradientElipse(CDC& dc)
{
    Graphics graphics(dc);

    // Create a path that consists of a single ellipse.
    GraphicsPath path;
    int scale = DPIScaleInt(1);
    path.AddEllipse(0, 80 * scale, 140 * scale, 70 * scale);

    // Use the path to construct a brush.
    PathGradientBrush pathBrush(&path);

    // Set the color at the center of the path to blue.
    pathBrush.SetCenterColor(Color(255, 0, 0, 255));

    // Set the color along the entire boundary of the path to aqua.
    Color colors[] = {Color(255, 0, 255, 255)};
    int count = 1;
    pathBrush.SetSurroundColors(colors, &count);
    graphics.FillEllipse(&pathBrush, 0, 80 * scale, 140 * scale, 70 * scale);
}

// Draw a solid ellipse.
void CGDIPlusView::DrawSolidElipse(CDC& dc)
{
    Graphics graphics(dc);

    SolidBrush solidBrush(Color(255, 255, 0, 0));
    int scale = DPIScaleInt(1);
    graphics.FillEllipse(&solidBrush, 160 * scale, 84 * scale, 100 * scale, 60 * scale);
}

// Draw a solid line.
void CGDIPlusView::DrawSolidLine(CDC& dc)
{
    Graphics graphics(dc);

    // Draw solid line
    Pen penLine(Color(255, 0, 0, 255));
    int scale = DPIScaleInt(1);
    graphics.DrawLine(&penLine, 10 * scale, 70 * scale, 200 * scale, 70 * scale);
}

// Draw some text.
void CGDIPlusView::DrawText(CDC& dc)
{
    Graphics graphics(dc);

    // Draw some text
    SolidBrush  brush(Color(255, 0, 0, 255));
    FontFamily  fontFamily(L"Times New Roman");
    int         scale = DPIScaleInt(1);
    Font        font(&fontFamily, 24.0f * scale, FontStyleRegular, UnitPixel);
    PointF      pointF(10.0f * scale, 20.0f * scale);

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
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

