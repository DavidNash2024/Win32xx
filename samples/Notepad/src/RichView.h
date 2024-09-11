/////////////////////////////
// RichView.h
//

#ifndef RICHVIEW_H
#define RICHVIEW_H


////////////////////////////////////////////////////
// CRichView manages a rich edit control. It is used
// as the view window for CMainFrame.
class CRichView : public CRichEdit
{
public:
    CRichView();
    virtual ~CRichView() override;
    int  CollatePages(const CDC& printerDC, int startChar = 0, int endChar = -1);
    void  DoPrint(LPCWSTR docName);
    void  PrintPage(CDC& dc, int page);
    void  QuickPrint(LPCWSTR docName);
    void  SetFontDefaults();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CRichView(const CRichView&) = delete;
    CRichView& operator=(const CRichView&) = delete;

    CRect GetPageRect(const CDC& dc);
    CRect GetPrintRect(const CDC& dc);
    std::vector<int> m_pageBreaks;
};

#endif // RICHVIEW_H
