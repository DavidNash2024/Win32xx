//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "FrameApp.h"
#include "enums.h"

using namespace Calc;

CView::CView() : m_inputDlg(IDD_INPUT), m_ymin(0), m_ymax(0)
{
}

//////////////////////////////////////////////////////////////////////
// Fills the m_points array
//////////////////////////////////////////////////////////////////////
bool CView::CalcPoints(double xmin, double xmax)
{
    assert(xmin < xmax);
    bool isValid = false;

    //Get the size of the client area of the view window
    CRect rect = GetClientRect();

    int numPoints = int(0.8 * MIN(rect.bottom, rect.right));
    numPoints = MAX(10, numPoints);

    // clear the current contents
    m_points.clear();

    double d_incr = (xmax - xmin) / (numPoints - 1.0);

    //fill the points array
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
    
    // find the first valid value
    std::vector<PointData>::iterator index = m_points.begin();
    while (index != m_points.end() && (*index).status != st_OK)
        index++;

    // find the maximum and minimum y values
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

        //if ymin and ymax are approximately equal ...
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

        //set true, as there are some valid points to plot
        isValid = true;
    }
    else
    {
        //  CString buffer = _T("The Expression was valid, but there were no valid points to plot! \n \n Try a different range of values for x");
        //  MessageBox(NULL, buffer, _T("Invalid Values"), MB_ICONEXCLAMATION);
        m_points.clear();
    }
    return isValid;
}

//////////////////////////////////////////////////////////////////////
// Plot the function to the screen
//  > sets up the device context, and calculates the scaling
//  > calls the PlotAxis and PlotFunction subroutines
//////////////////////////////////////////////////////////////////////
void CView::DoPlot(CDC& dc)
{
    if (m_points.size() == 0)
        return;

    // sanity check
    assert(GetXMin() < GetXMax());
    assert(m_ymin < m_ymax);

    // Choose a resolution based on the number of points to minamise rounding errors.
    int Resolution = m_points.size();
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

    PlotAxis(dc, xnorm, ynorm, xoffset, yoffset);
    PlotFunction(dc, xnorm, ynorm, xoffset, yoffset);
    DrawLabel(dc);
}

void CView::DrawLabel(CDC& dc)
{
    int fontHeight = 5 + int(.04 * m_points.size());
    dc.CreateFont(
        fontHeight, // height
        0, // width (0 means use default)
        0, // escapement (0 means none)
        0, // orientation (0 means none)
        FW_NORMAL, // "boldness" of font
        TRUE, // italic?  true or false
        FALSE, // underline? true or false
        FALSE, // strikeout?  true or false
        ANSI_CHARSET, // desired character set
        OUT_TT_PRECIS, // output precision - use TrueType only
        CLIP_DEFAULT_PRECIS, // clip precision - use default
        PROOF_QUALITY, // proof quality
        DEFAULT_PITCH | FF_DONTCARE, // pitch and family
        _T("Candara") // name of font face desired
    );
    CRect rc = GetClientRect();
    CString str = "f(x) = ";
    str += m_inputDlg.GetFunction();
    CSize sz = dc.GetTextExtentPoint32(str);

    CPoint pt((rc.Width() - sz.cx) / 2, rc.Height() / 50);
    dc.DPtoLP(pt, 1);
    dc.TextOut(pt.x, pt.y, str);
}

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn
{
    if (m_calc.Get_Status() != st_ERROR)
    {
        CalcPoints(GetXMin(), GetXMax());
        DoPlot(dc);
    }
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created
{
    TRACE("View window created\n");
}

//////////////////////////////////////////////////////////////////////
// Draws the x and y axis,  called by DoPlot
// > draw the major axis
// > draw the ticks
// > write the tick labels
//////////////////////////////////////////////////////////////////////
void CView::PlotAxis(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset)
{
    CRect rect;
    CRect rc = GetClientRect();
    CSize size(rc.right, rc.bottom);
    dc.CreatePen(PS_SOLID, 2, RGB(195, 0, 0));

    // create the font
    int fontHeight = 4 + int(.03 * m_points.size());
    dc.CreateFont(
        fontHeight, // height
        0, // width (0 means use default)
        0, // escapement (0 means none)
        0, // orientation (0 means none)
        FW_NORMAL, // "boldness" of font
        FALSE, // italic?  true or false
        FALSE, // underline? true or false
        FALSE, // strikeout?  true or false
        ANSI_CHARSET, // desired character set
        OUT_TT_PRECIS, // output precision - use TrueType only
        CLIP_DEFAULT_PRECIS, // clip precision - use default
        PROOF_QUALITY, // proof quality
        DEFAULT_PITCH | FF_DONTCARE, // pitch and family
        _T("Microsoft Sans Serif") // name of font face desired
    );

    // select the new font + color & store the old
    COLORREF oldcolor = dc.SetTextColor(RGB(0, 0, 0));
    int oldbkmode = dc.SetBkMode(TRANSPARENT);
    dc.SetTextAlign(TA_LEFT);

    //////////////////////////
    //plot the x axis
    //////////////////////////

    //adjust for rounding errors for m_ymin and m_ymax
    double ymax = m_ymax + .001 * (m_ymax - m_ymin);
    double ymin = m_ymin - .001 * (m_ymax - m_ymin);

//  double ymax = m_ymax;
//  double ymin = m_ymin;

    //ylinepos shifts the x axis down if the function doesn't cross it
    double ylinepos;
    if ((ymin * ymax <= 0) /*|| (ymin < (ymax - ymin)/10)*/)
        ylinepos = 0;
    else   
        ylinepos = ymin - .05 * (ymax - ymin);

    // Major axis
    {
        dc.MoveTo((int)(xnorm * (GetXMin() - xoffset)), (int)(ynorm * (ylinepos - yoffset)));
        dc.LineTo((int)(xnorm * (GetXMax() - xoffset)), (int)(ynorm * (ylinepos - yoffset)));
    }

    //Ticks
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
        dc.MoveTo(int(xnorm * (x - xoffset)), int(ynorm * (ylinepos - xtickheight - yoffset)));
        dc.LineTo(int(xnorm * (x - xoffset)), int(ynorm * (ylinepos + xtickheight - yoffset)));
        // Output tick text.
        // Avoid outputing zero if it would be written on a major axis
        if ((x != 0) || ((ymin * ymax >= 0) && (ymin >= 0)))
        {
            CString str;
            str.Format(_T("%g"), x);
            size = dc.GetTextExtentPoint32(str);  //Determine the size of the text
            dc.TextOut((int)(xnorm * (x - xoffset) - size.cx / 2), (int)(ynorm * (ylinepos - yoffset - 2 * xtickheight)), str);
        }
        x = ++xticknum * xtickgap;
    }

    ////////////////////
    //plot the y axis
    ////////////////////

    //xlinepos shifts the y axis left if the function doesn't cross it
    double xlinepos;
    if ((GetXMin() * GetXMax() <= 0) /*|| (GetXMin() < (GetXMax() - GetXMin()) / 10)*/)
        xlinepos = 0;
    else
        xlinepos = GetXMin() - .05 * (GetXMax() - GetXMin());

    //plot the major axis
    dc.MoveTo((int)(xnorm * (xlinepos - xoffset)), (int)(ynorm * (ymin - yoffset)));
    dc.LineTo((int)(xnorm * (xlinepos - xoffset)), (int)(ynorm * (ymax - yoffset)));

    //plot the ticks
    double ytickgap = floor(log10(ymax - ymin));
    ytickgap = pow((double)10, ytickgap);
    numticks = (int)((ymax - ymin) / ytickgap);

    if (numticks < 2) ytickgap = ytickgap / 10;
    else if (numticks < 4) ytickgap = ytickgap / 5;
    else if (numticks < 8) ytickgap = ytickgap / 2;
    double ytickheight = (GetXMax() - GetXMin()) / 100;

    int yticknum = (int)ceil(ymin / ytickgap);
    double y = yticknum * ytickgap;
    while (y <= ymax)
    {
        dc.MoveTo((int)(xnorm * (xlinepos - ytickheight - xoffset)), (int)(ynorm * (y - yoffset)));
        dc.LineTo((int)(xnorm * (xlinepos + ytickheight - xoffset)), (int)(ynorm * (y - yoffset)));

        // Output tick text.
        // Avoid outputing zero if it would be written on a major axis
        if ((y != 0) || ((GetXMin() * GetXMax() >= 0) && (GetXMin() >= 0)))
        {
            CString str;
            str.Format(_T("%g"), y);
            size = dc.GetTextExtentPoint32(str);  //Determine the size of the text
            dc.TextOut((int)(xnorm * (xlinepos - xoffset - 2 * ytickheight) - size.cx), (int)(ynorm * (y - yoffset) + size.cy / 2), str);
        }
        y = ++yticknum * ytickgap;
    }

    // cleanup
    dc.SetTextColor(oldcolor);
    dc.SetBkMode(oldbkmode);
}

//////////////////////////////////////////////////////////////////////
// Plots the function,   called by DoPlot
//////////////////////////////////////////////////////////////////////
void CView::PlotFunction(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset)
{
    CRect rect;
    dc.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

    // find the first valid value
    size_t index = 0;
    while ((m_points[index].status != st_OK) && (index < m_points.size() - 1))
        index++;

    //Draw the function
    dc.MoveTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
    while (index < m_points.size())
    {
        if (m_points[index].status == st_OK)
        {
            dc.LineTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
            //  CString str = CString(index) + (", ") + CString(m_points[index].x) + (", ") + CString(m_points[index].y);
            //  Trace(str); Trace("\n");
            index++;
        }
        else
        {   //skip plotting invalid points
            while ((index < m_points.size()) && (m_points[index].status != st_OK))
                index++;

            if (index < m_points.size())
                dc.MoveTo((int)(xnorm * (m_points[index].x - xoffset)), (int)(ynorm * (m_points[index].y - yoffset)));
        }
    }
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Here we set the Window class parameters.
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the type of window we create.

    // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

    // Set a background brush to white
    wc.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));

    // Set the default cursor
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// All window messages for this window pass through WndProc
{
    switch (msg)
    {
    case WM_SIZE:
        Invalidate();
        break;  // Also do default processing
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}



