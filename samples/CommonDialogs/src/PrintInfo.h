/////////////////////////////////////////
// PrintInfo.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef CPRINTINFO_H
#define CPRINTINFO_H

/////////////////////////////////////////////////////////////////////////
// CPrintInfo stores information about a print or print preview job.
// It emulates and largely uses the same notation as its MFC counterpart.
class CPrintInfo
{
public:
        CPrintInfo();
        ~CPrintInfo() = default;

    UINT    GetFromPage() const         { return m_nFromPage;}
    UINT    GetMaxPage() const          { return m_nMaxPage; }
    UINT    GetMinPage() const          { return m_nMinPage; }
    UINT    GetNCopies() const          { return m_nCopies; }
    UINT    GetToPage() const           { return m_nToPage; }
    void    InitInfo(CPrintDialog*, UINT, UINT, UINT, UINT, UINT);
    void    SetFromPage(UINT nFromPage) { m_nFromPage = nFromPage; }
    void    SetMaxPage(UINT nMaxPage)   { m_nMaxPage = nMaxPage; }
    void    SetMinPage(UINT nMinPage)   { m_nMinPage = nMinPage; }
    void    SetNCopies(UINT nCopies)    { m_nCopies = nCopies; }
    void    SetToPage(UINT nToPage)     { m_nToPage = nToPage; }

    // public data
    CPrintDialog* m_pPD;          // Pointer to print dialog.
    BOOL    m_bDocObject;         // TRUE if a DocObject (not supported).
    BOOL    m_bDirect;            // TRUE if bypassing Print Dialog.
    BOOL    m_bPreview;           // TRUE if in preview mode.
    BOOL    m_bContinuePrinting;  // Set FALSE to end printing.
    UINT    m_nCopies;            // Number of copies (not supported).
    UINT    m_nFromPage;          // First printed page, 1 based.
    UINT    m_nMaxPage;           // Maximum page limit.
    UINT    m_nMinPage;           // Minimum page limit.
    UINT    m_nToPage;            // Last printed page.
    UINT    m_nCurPage;           // Current page.
    UINT    m_nNumPreviewPages;   // (not supported).
    LPVOID  m_lpUserData;         // Pointer to user created struct.
    CRect   m_rectDraw;           // The current usable page area.
    CString m_strPageDesc;        // Format string for page number display.
    UINT    m_nMargin;            // Page margin, in twips.

    // Currently not supported. These members are included for completeness and
    // future use.
    DWORD   m_dwFlags;            // Flags for DocObject print operations.
    UINT    m_nOffsetPage;        // First page offset in combined Doc job.
};

#endif // CPRINTINFO_H
