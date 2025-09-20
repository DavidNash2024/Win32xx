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
    CRichView() = default;
    virtual ~CRichView() = default;
    int   CollatePages(const CDC& printerDC);
    void  DoPrint(LPCWSTR docName);
    void  PrintPage(CDC& dc, int page);
    void  QuickPrint(LPCWSTR docName);
    void  SetDefaultPrintOptions();
    void  SetFontDefaults();
    std::vector<int> SetPagesToPrint(const CDC& printerDC);

protected:
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT, WPARAM, LPARAM) override;

private:
    CRichView(const CRichView&) = delete;
    CRichView& operator=(const CRichView&) = delete;

    CRect GetPageRect(const CDC& dc);
    CRect GetPrintRect(const CDC& dc);
    std::vector<int> m_pageBreaks;
    CCustomPrintDlg m_printDialog;
};

#endif // RICHVIEW_H
