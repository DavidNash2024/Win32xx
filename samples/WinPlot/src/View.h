/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Calc.h"
#include "InputDlg.h"


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    struct PointData
    {
        PointData(double x, double y, int status) : x(x), y(y), status(status)
        {}
        PointData() : x(0), y(0), status(0) {}

        double x;
        double y;
        int status;
    };
    CView();
    virtual ~CView(){}

    double GetXMin() { return m_inputDlg.GetMin(); }
    double GetXMax() { return m_inputDlg.GetMax(); }
    Calc::Calculator& GetCalc() { return m_calc; }
    CInputDlg& GetInput() { return m_inputDlg; }

    void CalcPoints(double xmin, double xmax);
    void DoPlot(CDC& dc);
    void DrawLabel(CDC& dc);
    void PlotXAxis(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset);
    void PlotYAxis(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset);
    void PlotFunction(CDC& dc, double xnorm, double ynorm, double xoffset, double yoffset);
    void PrepareDC(CDC& dc);

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    Calc::Calculator m_calc;
    std::vector<PointData> m_points;  // vector of Data, stores x, y, & status
    CInputDlg m_inputDlg;

    double m_ymin;
    double m_ymax;
};


#endif // VIEW_H
