
#include "stdafx.h"
#include "Doc.h"


BOOL CDoc::FileLoad(LPCTSTR szFilename)
{
    BOOL bResult = FALSE;

    // Return CDoc data to default

    try
    {
        CArchive ar(szFilename, CArchive::load);
        ar >> *this;        // Uses the Serialize function
        bResult = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
        
        // Return CDoc data to default
    }

    return bResult;
}

BOOL CDoc::FileStore(LPCTSTR /* szFilename */)
{
    BOOL bResult = FALSE;

//  try
//  {
//      CArchive ar(szFilename, CArchive::store);
//      ar << *this;        // Uses the Serialize function
//      bResult = TRUE;
//  }
//  catch (const CFileException &e)
//  {
//      // An exception occurred. Display the relevant information.
//      ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
//  }

    return bResult;
}

void CDoc::Serialize(CArchive& /* ar */)
// Uses CArchive to stream data to or from a file
{

//  if (ar.IsStoring())
//  {
//      // Store data in archive
//      ar << m_MemberVariable;
//  }
//  else
//  {
//      // Load data from archive
//      ar >> m_MemberVariable;
//  }

}

