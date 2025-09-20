/////////////////////////////
// CDoc.h
//

#include "stdafx.h"
#include "Doc.h"

/////////////////////////////
// CDoc function definitions.
//
BOOL CDoc::FileLoad(LPCWSTR filename)
{
    BOOL isLoaded = FALSE;

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;        // Uses the Serialize function
        isLoaded = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        TaskDialogBox(nullptr, e.GetText(), L"Failed to Load File", TD_WARNING_ICON);

        // Return CDoc data to default
    }

    return isLoaded;
}

BOOL CDoc::FileStore(LPCWSTR /* filename */)
{
    BOOL isStored = FALSE;

//  try
//  {
//      CArchive ar(szFilename, CArchive::store);
//      ar << *this;        // Uses the Serialize function
//      IsStored = TRUE;
//  }
//  catch (const CFileException &e)
//  {
//      // An exception occurred. Display the relevant information.
//      TaskDialogBox(nullptr, e.GetText(), L"Failed to Save File", TD_WARNING_ICON);
//  }

    return isStored;
}

void CDoc::Serialize(CArchive& /* ar */)
// Uses CArchive to stream data to or from a file
{

//  if (ar.IsStoring())
//  {
//      // Store data in archive
//      ar << m_memberVariable;
//  }
//  else
//  {
//      // Load data from archive
//      ar >> m_memberVariable;
//  }

}

