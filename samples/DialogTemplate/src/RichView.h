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
    CRichView();
    virtual ~CRichView(){}
    UINT CollatePages();
    void DoPrint(LPCTSTR docName);
    void PrintPage(CDC& dc, UINT page = 1);
    void QuickPrint(LPCTSTR docName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CRichView(const CRichView&);                // Disable copy construction
    CRichView& operator = (const CRichView&);   // Disable assignment operator

    // Message handlers
    LRESULT OnDropFiles(UINT, WPARAM wparam, LPARAM);

    // Private member functions
    CRect GetPageRect();
    CRect GetPrintRect();

    // Private members variables
    std::vector<int> m_pageBreaks;
};


#endif // VIEW_H
