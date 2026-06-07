/////////////////////////////
// RichView.h
//

#ifndef RICHVIEW_H
#define RICHVIEW_H

/////////////////////////////////////
// Declaration of the CRichEdit class
//
class CRichView : public CRichEdit
{
public:
    CRichView();
    virtual ~CRichView();
    void AppendBigText(LPCTSTR text);
    void AppendBigWarning(LPCTSTR text);
    void AppendSmallWarning(LPCTSTR text);
    void AppendSmallText(LPCTSTR text);
    UINT CollatePages();
    void DoPrint(LPCTSTR docName);
    void PrintPage(CDC& dc, int page);
    void QuickPrint(LPCTSTR docName);
    void UseLargeFont(COLORREF color);
    void UseSmallFont(COLORREF color);

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CRect GetPageRect();
    CRect GetPrintRect();

    std::vector<int> m_pageBreaks;
};

#endif // RICHVIEW_H
