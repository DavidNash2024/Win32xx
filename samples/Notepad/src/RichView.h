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
    UINT  CollatePages();
    void  DoPrint(LPCTSTR docName);
    void  PrintPage(CDC& dc, UINT page);
    void  QuickPrint(LPCTSTR docName);
    void  SetFontDefaults();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CRichView(const CRichView&);               // Disable copy construction
    CRichView& operator=(const CRichView&);    // Disable assignment operator

    CRect GetPageRect();
    CRect GetPrintRect();
    std::vector<int> m_pageBreaks;
};

#endif // RICHVIEW_H
