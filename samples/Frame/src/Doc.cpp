
#include "stdafx.h"
#include "Doc.h"


void CDoc::FileLoad(LPCTSTR filename)
{
    // Return CDoc data to default

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;        // Uses the Serialize function
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
    }
}

void CDoc::FileStore(LPCTSTR /* filename */)
{

//  try
//  {
//      CArchive ar(filename, CArchive::store);
//      ar << *this;        // Uses the Serialize function
//      result = TRUE;
//  }
//  catch (const CFileException &e)
//  {
//      // An exception occurred. Display the relevant information.
//      ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
//  }

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

