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
    virtual ~CRichView();
    void  ClearPrintOptions();
    UINT  CollatePages();
    void  DoPrint(LPCTSTR docName);
    void  PrintPage(CDC& dc, UINT page);
    void  QuickPrint(LPCTSTR docName);
    virtual void SetFontDefaults();
    std::vector<int> SetPagesToPrint();
    void SetPrintOptions();

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CRect GetPageRect();
    CRect GetPrintRect();

    CFont   m_font;
    std::vector<int> m_pageBreaks;
    CPrintDialogEx m_printDialog;
    PRINTPAGERANGE m_pageRanges[10];
};

#endif // RICHVIEW_H
