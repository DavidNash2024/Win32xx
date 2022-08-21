/////////////////////////////
// RichView.h
//

#ifndef RICHVIEW_H
#define RICHVIEW_H

// include CMyPrintDialogEx and CPrintParent
#include "PrintDialog.h"


////////////////////////////////////////////////////
// CRichView manages a rich edit control. It is used
// as the view window for CMainFrame.
class CRichView : public CRichEdit
{
public:
    CRichView();
    virtual ~CRichView();
    void  ClearPrintOptions();
    UINT  CollatePages(const CDC& printerDC);
    void  DoPrint(LPCTSTR docName);
    LRESULT OnSettingChange(UINT msg, WPARAM wparam, LPARAM lparam);
    void  PrintFooter(CDC& dc, UINT page);
    void  PrintPage(CDC& dc, UINT page);
    void  QuickPrint(LPCTSTR docName);
    void  SetFontDefaults();
    std::vector<UINT> SetPagesToPrint(const CDC& printerDC);
    void SetPrintOptions();

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CRichView(const CRichView&);                // Disable copy construction
    CRichView& operator = (const CRichView&);   // Disable assignment operator

    CRect GetPageRect(const CDC& printerDC);
    CRect GetPrintRect(const CDC& printerDC);

    CFont   m_font;
    std::vector<int> m_pageBreaks;
    CMyPrintDialogEx m_printDialog;
    PRINTPAGERANGE m_pageRanges[10];
};

#endif // RICHVIEW_H
