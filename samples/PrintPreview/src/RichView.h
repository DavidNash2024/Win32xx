/////////////////////////////
// RichView.h
//

#ifndef RICHVIEW_H
#define RICHVIEW_H


/////////////////////////////////////
// CRichView manages a rich edit control. It is used
// as the view window by CMainFrame.
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
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void OnAttach();

private:
    CRect GetPageRect();
    CRect GetPrintRect();

    CFont   m_font;
    std::vector<int> m_pageBreaks;
};

#endif // RICHVIEW_H
