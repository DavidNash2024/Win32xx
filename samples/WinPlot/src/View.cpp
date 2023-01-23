/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "FrameApp.h"
#include "enums.h"

using namespace Calc;

//////////////////////////////
// CView function definitions.
//

// Constructor
CView::CView() : m_inputDlg(IDD_INPUT), m_ymin(0), m_ymax(0)
{
}

// Fills the m_points vector with values to plot.
void CView::CalcPoints(double xmin, double xmax)
{
    assert(xmin < xmax);
    m_points.clear();

    // Get the size of the client area of the view window.
    CRect rect = GetClientRect();

    int numPoints = int(0.8 * MIN(rect.bottom, rect.right));
    numPoints = MAX(10, numPoints);
    double d_incr = (xmax - xmin) / (numPoints - 1.0);

    // Fill the points vector.
    for (int i = 0; i < numPoints; i++)
    {
        double x;
        if (i == numPoints - 1)
            x = xmax;
        else
            x = xmin + i * d_incr;

        m_calc.SetVar("x", x);
        double y = m_calc.Eval();
        m_points.push_back(PointData(x, y, m_calc.Get_Status()));
    }

    // Find the first valid value.
    std::vector<PointData>::iterator index = m_points.begin();
    while (index != m_points.end() && (*index).status != st_OK)
        index++;

    // Find the maximum and minimum y values.
    if (index != m_points.end())
    {
        double ymin = (*index).y;
        double ymax = (*index).y;

        for (std::vector<PointData>::iterator i = index; i != m_points.end(); ++i)
        {
            if ((*i).status == st_OK)
            {
                ymin = MIN(ymin, (*i).y);
                ymax = MAX(ymax, (*i).y);
            }
        }

        // If ymin and ymax are approximately equal ...
        if ((fabs((ymax - ymin) / (ymax + ymin)) < 1.0e-010) || (ymin == ymax))
        {
            ymin = ymin * 2;
            ymax = ymax * 2;
            if (ymin == 0)
            {
                ymin = -1;
                ymax = 1;
            }
            else if (ymin > 0) ymin = 0;
            else ymax = 0;
        }

        m_ymin = ymin;
        m_ymax = ymax;
    }
    else
    {
        m_points.clear();
    }
}

// Plot the function to the window.
void CView::DoPlot(CDC& dc)
{
    if (m_points.size() == 0)
        return;

    // sanity check
    assert(GetXMin() < GetXMax());
    assert(m_ymin < m_ymax);

    // Choose a resolution based on the number of points to minamise rounding errors.
    int Resolution = static_cast<int>(m_points.size());
    dc.SetMapMode(MM_ISOTROPIC);    // Scale X and Y equally
    dc.SetWindowExtEx(Resolution, Resolution);

    // Scale the viewport to 80% of the client area window size.
    CRect rect = GetClientRect();
    dc.SetViewportExtEx(int(rect.right * .8), int(-rect.bottom * .8));

    // Set the viewport origin to the centre of the screen.
    dc.SetViewportOrgEx(int(rect.right / 2.), int(rect.bottom / 2));

    // Normalise or scale the X and Y values.
    double xnorm = Resolution / (GetXMax() - GetXMin());
    double ynorm = Resolution / (m_ymax - m_ymin);

    // Offset from the origin
    double xoffset = (GetXMin() + GetXMax()) / 2;
    double yoffset = (m_ymin + m_ymax) / 2;

    PrepareDC(dc);
    PlotXAxis(dc, xnorm, ynorm, xoffset, yoffset);
    PlotYAxis(dc, xnorm, ynorm, xoffset, yoffset);
    PlotFunction(dc, xnorm, ynorm, xoffset, yoffset);
    DrawLabel(dc);
}

void CView::DrawLabel(CDC& dc)
{
    // Select the font.
    int pointSize = 20 + int(.2 * m_points.size());
    dc.CreatePointFont(pointSize, _T("Candara"));

    // Draw the text.
    CRect rc = GetClientRect();
    CString str = "f(x) = ";
    str += m_inputDlg.GetFunction();
    CSize sz = dc.GetTextExtentPoint32(str);
    CPoint pt((rc.Width() - sz.cx) / 2, rc.Height() / 50);
    dc.DPtoLP(pt, 1);
    dc.TextOut(pt.x, pt.y, str);
}

// Called when part or all of the window needs to be redrawn.
void CView::OnDraw(CDC& dc)
{
    if (m_calc.Get_Status() != st_ERROR)
    {
        CalcPoints(GetXMin(), GetXMax());
        DoPlot(dc);
    }
}

// OnInitialUpdate is called after the window is created.
void CView::OnInitialUpdate()
{
    TRACE("View window created\n");
}

void CView::PrepareDC(CDC& dc)
{
    // Select the pen.
    dc.CreatePen(PS_SOLID, 3, RGB(195, 0, 0));

    // Select the font.
    int pointSize = 20 + int(.15 * m_points.size());
    dc.CreatePointFont(pointSize, _T("Microsoft Sans Serif"));

    // Select the color.
    dc.SetTextColor(RGB(0, 0, 0));
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextAlign(TA_LEFT);
}

// Draws the x axis, including the ticks and tick labels.
void CView::PlotXAxis(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset)
{
    CRect rc = GetClientRect();
    CSize size(rc.right, rc.bottom);

    // Adjust for rounding errors for m_ymin and m_ymax.
    double ymax = m_ymax + .001 * (m_ymax - m_ymin);
    double ymin = m_ymin - .001 * (m_ymax - m_ymin);

    // Shift the x axis down if the function doesn't cross it.
    double ylinepos;
    if ((ymin * ymax <= 0))
        ylinepos = 0;
    else
        ylinepos = ymin - .05 * (ymax - ymin);

    // Draw the major axis
    dc.MoveTo((int)(xnorm * (GetXMin() - xoffset)), (int)(ynorm * (ylinepos - yoffset)));
    dc.LineTo((int)(xnorm * (GetXMax() - xoffset)), (int)(ynorm * (ylinepos - yoffset)));

    // Draw the ticks.
    double xtickgap = floor(log10(GetXMax() - GetXMin()));
    xtickgap = pow(double(10), xtickgap);
    int numticks = int((GetXMax() - GetXMin()) / xtickgap);
    if (numticks < 2) xtickgap = xtickgap / 10;
    else if (numticks < 4) xtickgap = xtickgap / 5;
    else if (numticks < 8) xtickgap = xtickgap / 2;
    double xtickheight = (ymax - ymin) / 100;

    int xticknum = int(ceil(GetXMin() / xtickgap));
    double x = xticknum * xtickgap;
    while (x <= GetXMax())
    {
        // Draw the ticks.
        dc.MoveTo(int(xnorm * (x - xoffset)), int(ynorm * (ylinepos - xtickheight - yoffset)));
        dc.LineTo(int(xnorm * (x - xoffset)), int(ynorm * (ylinepos + xtickheight - yoffset)));

        // Draw tick text.
        // Avoid outputting zero if it would be written on a major axis.
        if ((x != 0) || ((ymin * ymax >= 0) && (ymin >= 0)))
        {
            CString str;
            str.Format(_T("%g"), x);
            size = dc.GetTextExtentPoint32(str);  //Determine the size of the text.
            dc.TextOut((int)(xnorm * (x - xoffset) - size.cx / 2), (int)(ynorm * (ylinepos - yoffset - 2 * xtickheight)), str);
        }
        x = ++xticknum * xtickgap;
    }
}

// Draws the y axis, including the ticks and tick labels.
void CView::PlotYAxis(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset)
{
    CRect rc = GetClientRect();
    CSize size(rc.right, rc.bottom);

    // Adjust for rounding errors for m_ymin and m_ymax.
    double ymax = m_ymax + .001 * (m_ymax - m_ymin);
    double ymin = m_ymin - .001 * (m_ymax - m_ymin);

    // Shift the y axis left if the function doesn't cross it.
    double xlinepos;
    if ((GetXMin() * GetXMax() <= 0))
        xlinepos = 0;
    else
        xlinepos = GetXMin() - .05 * (GetXMax() - GetXMin());

    // Draw the major axis.
    dc.MoveTo((int)(xnorm * (xlinepos - xoffset)), (int)(ynorm * (ymin - yoffset)));
    dc.LineTo((int)(xnorm * (xlinepos - xoffset)), (int)(ynorm * (ymax - yoffset)));

    double ytickgap = floor(log10(ymax - ymin));
    ytickgap = pow((double)10, ytickgap);
    int numticks = (int)((ymax - ymin) / ytickgap);

    if (numticks < 2) ytickgap = ytickgap / 10;
    else if (numticks < 4) ytickgap = ytickgap / 5;
    else if (numticks < 8) ytickgap = ytickgap / 2;
    double ytickwidth = (GetXMax() - GetXMin()) / 100;

    int yticknum = (int)ceil(ymin / ytickgap);
    double y = yticknum * ytickgap;
    while (y <= ymax)
    {
        // Draw the ticks.
        dc.MoveTo((int)(xnorm * (xlinepos - ytickwidth - xoffset)), (int)(ynorm * (y - yoffset)));
        dc.LineTo((int)(xnorm * (xlinepos + ytickwidth - xoffset)), (int)(ynorm * (y - yoffset)));

        // Draw tick text.
        // Avoid outputting zero if it would be written on a major axis.
        if ((y != 0) || ((GetXMin() * GetXMax() >= 0) && (GetXMin() >= 0)))
        {
            CString str;
            str.Format(_T("%g"), y);
            size = dc.GetTextExtentPoint32(str);  //Determine the size of the text.
            dc.TextOut((int)(xnorm * (xlinepos - xoffset - 2 * ytickwidth) - size.cx), (int)(ynorm * (y - yoffset) + size.cy / 2), str);
        }
        y = ++yticknum * ytickgap;
    }
}

// Plots the function.
void CView::PlotFunction(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset)
{
    CRect rect;
    dc.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));

    // Find the first valid value.
    size_t index = 0;
    while ((m_points[index].status != st_OK) && (index < m_points.size() - 1))
        index++;

    // Draw the function.
    dc.MoveTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
    while (index < m_points.size())
    {
        if (m_points[index].status == st_OK)
        {
            dc.LineTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
            index++;
        }
        else
        {   // Skip plotting invalid points.
            while ((index < m_points.size()) && (m_points[index].status != st_OK))
                index++;

            if (index < m_points.size())
                dc.MoveTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
        }
    }
}

// Set the CREATESTRUCT parameters before the window is created.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

// Set the WNDCLASS parameters before the window is created.
void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

    // Set a background brush to white
    wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));

    // Set the default cursor
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

// All window messages for this window pass through WndProc
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
            Invalidate();
            break;  // Also do default processing.
        }

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
