/////////////////////////////////////////
// PrintInfo.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "PrintInfo.h"

///////////////////////////////////
// CPrintInfo function definitions.
//

CPrintInfo::CPrintInfo()
{
    m_pPD = nullptr;
    m_bPreview   = FALSE;           // Initialize to not preview.
    m_bDirect    = FALSE;           // Initialize to not direct.
    m_bDocObject = FALSE;           // Initialize to not print.
    m_bContinuePrinting = FALSE;    // Assume it is not yet OK to print.

    m_dwFlags = 0;                  // Not supported.
    m_nOffsetPage = 0;              // Not supported.

    m_nCopies    = 1;               // Number of copies to print.
    m_nFromPage  = 1;               // First printed page, 1 based.
    m_nMaxPage   = 0xffff;          // Maximum page limit.
    m_nMinPage   = 1;               // Minimum page limit.
    m_nToPage    = 0xffff;          // Last printed page.
    m_nCurPage   = 0;               // Current page

    m_lpUserData = nullptr;         // Pointer to user created struct.
    m_nNumPreviewPages = 0;         // Not supported.
    m_rectDraw.SetRect(0, 0, 0, 0); // Current usable page area.
    m_strPageDesc = L"Page %u";     // Format string for page number display.
    m_nMargin     = 0;              // Page margin, in twips.
}

// Set initial values for the CPrintInfo object.
void CPrintInfo::InitInfo(CPrintDialog* pPD, UINT fromPage, UINT toPage,
    UINT minPage, UINT maxPage, UINT nCopies)
{
    assert(pPD);
    m_pPD        = pPD;
    PRINTDLG pd  = m_pPD->GetParameters();
    pd.nCopies   = static_cast<WORD>(nCopies);
    pd.nFromPage = static_cast<WORD>(fromPage);
    pd.nToPage   = static_cast<WORD>(toPage);
    pd.nMinPage  = static_cast<WORD>(minPage);
    pd.nMaxPage  = static_cast<WORD>(maxPage);
    m_pPD->SetParameters(pd);
}

