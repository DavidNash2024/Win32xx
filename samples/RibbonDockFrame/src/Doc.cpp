

#include "stdafx.h"
#include "Doc.h"
#include "View.h"


BOOL CDoc::FileOpen(LPCTSTR filename)
{
    GetAllPoints().clear();
    BOOL result = FALSE;

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;
        result = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
        
        GetAllPoints().clear();
    }

    return result;
}

BOOL CDoc::FileSave(LPCTSTR filename)
{
    BOOL result = TRUE;

    try
    {
        CArchive ar(filename, CArchive::store);
        ar << *this;
    }
    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
        result = FALSE;
    }

    return result;
}

void CDoc::Serialize(CArchive &ar)
// Uses CArchive to stream data to or from a file
{

    if (ar.IsStoring())
    {
        // Store the number of points
        UINT points = GetAllPoints().size();
        ar << points;
        
        // Store the PlotPoint data
        std::vector<PlotPoint>::iterator iter;
        for (iter = GetAllPoints().begin(); iter < GetAllPoints().end(); ++iter)
        {
            PlotPoint pp = (*iter);
            ArchiveObject ao( &pp, sizeof(PlotPoint) );
            ar << ao;
        }
    }
    else
    {
        UINT points;
        PlotPoint pp;
        GetAllPoints().clear();

        // Load the number of points
        ar >> points;

        // Load the PlotPoint data
        for (UINT u = 0; u < points; ++u)
        {
            ArchiveObject ao( &pp, sizeof(pp) );
            ar >> ao;
            GetAllPoints().push_back(pp);
        }
    }

}

void CDoc::StorePoint(PlotPoint& pp)
{
    m_points.push_back(pp); //Add the point to the vector
}