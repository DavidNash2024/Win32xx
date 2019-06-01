//////////////////////////////////////////////
// RichView.h

#ifndef RICHVIEW_H
#define RICHVIEW_H


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
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void OnAttach();

private:
    CRect GetPageRect();
    CRect GetPrintRect();

    CFont   m_font;
    std::vector<int> m_pageBreaks;
};

#endif // RICHVIEW_H
