/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


///////////////////////////////////////////////////////
// CRichview manages CMainFrame's Richedit view window.
class CRichView : public CRichEdit
{
public:
    CRichView() = default;
    virtual ~CRichView() override = default;
    int CollatePages();
    void DoPrint(LPCWSTR docName);
    void PrintPage(CDC& dc, int page = 1);
    void QuickPrint(LPCWSTR docName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach() override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CRichView(const CRichView&) = delete;
    CRichView& operator=(const CRichView&) = delete;

    // Message handlers
    LRESULT OnDropFiles(UINT, WPARAM wparam, LPARAM);

    // Private member functions
    CRect GetPageRect();
    CRect GetPrintRect();

    // Private members variables
    std::vector<int> m_pageBreaks;
};


#endif // VIEW_H
