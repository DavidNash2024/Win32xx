/////////////////////////////////////////
// RichEditView.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef CRICHEDITVIEW_H
#define CRICHEDITVIEW_H

#include "PrintInfo.h"

///////////////////////////////////////////////////////////////////////////////
// This class serves as both the view object as well as the document repository
// and manager. It also contains the print loop.
class CRichEditView : public CRichEdit
{
public:
    CRichEditView();
    virtual ~CRichEditView() override = default;

    void    Clean();
    void    DoPrintRichView(LPCWSTR);
    void    DoPrintView();
    BOOL    IsSelected();
    void    SetColors(COLORREF txfg, COLORREF txbg, COLORREF bg);
    void    SetFont(HFONT hFont, BOOL bRedraw) const;
    void    SetPrintPath(LPCWSTR s) { m_printPath = s;}
    void    SetWrapping(int wrap);
    BOOL    StreamInFile(const CFile& file);
    BOOL    StreamOutFile(const CFile& file);

protected:
    virtual void OnAttach() override;

private:
    CRichEditView(const CRichEditView&) = delete;
    CRichEditView& operator=(const CRichEditView&) = delete;

    BOOL    DoPreparePrinting(CPrintInfo& info);
    void    GetPageBreaks(CPrintInfo&);
    void    OnBeginPrinting(CDC& DC, CPrintInfo& info);
    void    OnEndPrinting(CDC& DC, CPrintInfo& info);
    void    OnPrepareDC(CDC& DC, CPrintInfo& info);
    BOOL    OnPreparePrinting(CPrintInfo& info);
    void    OnPrint(CDC& DC, CPrintInfo& info);
    void    SetAppBanding(BOOL b) { m_isAppBanded = b;}

    static  DWORD CALLBACK StreamInCallback(DWORD dwCookie, LPBYTE pbBuff,
        LONG cb, LONG *pcb);
    static  DWORD CALLBACK StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff,
        LONG cb, LONG *pcb);

    long    m_textLength;
    CString m_docPath;                // Document path name.
    CString m_printPath;              // Print-to-file path (not supported).
    CString m_dataType;               // DOCINFO data type for the print job.
    BOOL    m_isAppBanded;            // TRUE if app banding supported.
    FORMATRANGE m_fr;                 // Rich edit format range struct.
    std::vector<int> m_firstPageChar; // First character of each page.
};

#endif  // CRICHEDITVIEW_H
