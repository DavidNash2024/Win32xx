/////////////////////////////
// RichView.h
//

#ifndef RICHVIEW_H
#define RICHVIEW_H


#include "CustomPrintDlg.h"


////////////////////////////////////////////////////
// CRichView manages a rich edit control. It is used
// as the view window for CMainFrame.
class CRichView : public CRichEdit
{
public:
    CRichView();
    virtual ~CRichView();
    int   CollatePages(const CDC& printerDC);
    void  DoPrint(LPCTSTR docName);
    void  PrintFooter(CDC& dc, UINT page);
    void  PrintPage(CDC& dc, UINT page);
    void  QuickPrint(LPCTSTR docName);
    void  SetDefaultPrintOptions();
    void  SetFontDefaults();
    std::vector<UINT> SetPagesToPrint(const CDC& printerDC);

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

private:
    CRichView(const CRichView&);               // Disable copy construction
    CRichView& operator=(const CRichView&);    // Disable assignment operator

    CRect GetPageRect(const CDC& printerDC);
    CRect GetPrintRect(const CDC& printerDC);
    std::vector<int> m_pageBreaks;
    CCustomPrintDlg m_printDialog;
};

#endif // RICHVIEW_H
