/////////////////////////////////////////
// RichView.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef RICHVIEW_H
#define RICHVIEW_H

// Word wrap options.
enum WordWrapType
{
    WRAP_NONE    = 0,
    WRAP_WINDOW  = 1,
    WRAP_PRINTER = 2
};

//////////////////////////////////////////////////////////////////////
// This class is the customized class derived from the CRichEdit class
// for the PrintPreview sample program utilizing a rich edit window.
class CRichView : public CRichEdit
{
public:
    CRichView() = default;
    virtual ~CRichView() override = default;

    UINT GetPageBreaks(CDC& dcPrinter);
    BOOL GetNewFont() const;
    void PrintDC(UINT, CDC&, CDC&);
    BOOL ReadFile(LPCWSTR filePath);
    void SetFontDefaults();
    void SetDocName(LPCWSTR &filePath) { m_docPath = filePath;}
    void PrintPages(CPrintDialog& printDlg);
    void WordWrap(WordWrapType setting);
    BOOL WriteFile(LPCWSTR filePath);

protected:
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;

private:
    CRichView(const CRichView&) = delete;
    CRichView& operator=(const CRichView&) = delete;

    // Static callback functions.
    static  DWORD CALLBACK RVStreamInCallback(DWORD cookie, LPBYTE pbBuff,
        LONG cb, LONG *pcb);
    static  DWORD CALLBACK RVStreamOutCallback(DWORD cookie, LPBYTE pbBuff,
        LONG cb, LONG *pcb);

    CFont   m_font;
    CString m_docPath;
    std::vector<int> m_pageBreaks;  // Page starting position list.
};

#endif // RICHVIEW_H
