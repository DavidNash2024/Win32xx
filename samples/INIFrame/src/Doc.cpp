/////////////////////////////
// Doc.cpp
//

#include "stdafx.h"
#include "Doc.h"

////////////////////////////
// CDoc function definitions
//

// An example of how to load document data.
BOOL CDoc::FileLoad(LPCWSTR filename)
{
    BOOL result = FALSE;

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;        // Uses the Serialize function
        result = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(nullptr, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        // Return CDoc data to default
    }

    return result;
}

// An example of how to save document data.
BOOL CDoc::FileStore(LPCWSTR /* filename */)
{
    BOOL result = FALSE;
//  try
//  {
//      CArchive ar(filename, CArchive::store);
//      ar << *this;        // Uses the Serialize function
//      result = TRUE;
//  }
//  catch (const CFileException &e)
//  {
//      // An exception occurred. Display the relevant information.
//      ::MessageBox(nullptr, e.GetText(), L"Failed to Save File", MB_ICONWARNING);
//  }

    return result;
}

// Uses CArchive to stream data to or from a file
void CDoc::Serialize(CArchive& /* ar */)
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

