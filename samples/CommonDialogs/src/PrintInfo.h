/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]             (PrintInfo.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CPrintInfo class for
    applications using the Win32++ Windows interface classes, Copyright
    (c) 2005-2016 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Special Conventions:

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Declaration of the CColorChoice class

*******************************************************************************/

#ifndef CPRINTINFO_H
#define CPRINTINFO_H

/*============================================================================*/
    class
CPrintInfo                                                              /*

*-----------------------------------------------------------------------------*/
{
    public:
            CPrintInfo();
            ~CPrintInfo();
            
        UINT    GetFromPage() const { return m_nFromPage;}
        UINT    GetMaxPage() const  { return m_nMaxPage; }
        UINT    GetMinPage() const  { return m_nMinPage; }
        UINT    GetNCopies() const  { return m_nCopies; }
        UINT    GetToPage() const   { return m_nToPage; }
        void    InitInfo(CPrintDialog*, UINT, UINT, UINT, UINT, UINT);
        void    SetFromPage(UINT nFromPage) { m_nFromPage = nFromPage; }
        void    SetMaxPage(UINT nMaxPage)   { m_nMaxPage = nMaxPage; }
        void    SetMinPage(UINT nMinPage)   { m_nMinPage = nMinPage; }
        void    SetNCopies(UINT nCopies)    { m_nCopies = nCopies; }
        void    SetToPage(UINT nToPage)     { m_nToPage = nToPage; }

    //  UINT    GetOffsetPage() const; // (not supported)

          // public data
        CPrintDialog* m_pPD;    // pointer to print dialog
        BOOL    m_bDocObject,   // TRUE if a DocObject (not supported)
            m_bDirect,  // TRUE if bypassing Print Dialog
            m_bPreview, // TRUE if in preview mode
            m_bContinuePrinting;// set FALSE to end printing
        UINT    m_nCopies,      // number of copies (not supported)
            m_nFromPage,    // first printed page, 1 based
            m_nMaxPage,     // maximum page limit
            m_nMinPage,     // minimum page limit
            m_nToPage,      // last printed page
            m_nCurPage, // current page
            m_nNumPreviewPages; // (not supported)
        LPVOID  m_lpUserData;   // pointer to user created struct       
        CRect   m_rectDraw; // rect of current usable page area     
        CString m_strPageDesc;  // format string for page number display
        UINT    m_nMargin;  // page margin, in twips

          // these only valid if m_bDocObject: currently not supported
        DWORD   m_dwFlags;  // flags for DocObject print operations
        UINT    m_nOffsetPage;  // first page offset in combined Doc job
};
/*----------------------------------------------------------------------------*/
#endif //CPRINTINFO_H
