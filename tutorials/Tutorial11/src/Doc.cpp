//////////////////////////////////
// Doc.cpp

#include "stdafx.h"
#include "Doc.h"


// Loads the PlotPoint data from the archive.
// Throws an exception if unable to read the file.
void CDoc::FileOpen(LPCWSTR fileName)
{
    GetAllPoints().clear();
    CArchive ar(fileName, CArchive::load);
    ar >> *this;
}

// Stores the PlotPoint data in the archive.
// Throws an exception if unable to save the file.
void CDoc::FileSave(LPCWSTR fileName)
{
    CArchive ar(fileName, CArchive::store);
    ar << *this;
}

// Uses CArchive to stream data to or from a file.
void CDoc::Serialize(CArchive &ar)
{

    if (ar.IsStoring())
    {
        // Store the number of points.
        UINT points = UINT(GetAllPoints().size());
        ar << points;

        // Store the PlotPoint data.
        for (PlotPoint& pp : GetAllPoints())
        {
            ArchiveObject ao(&pp, sizeof(pp));
            ar << ao;
        }
    }
    else
    {
        UINT points;
        PlotPoint pp;
        GetAllPoints().clear();

        // Load the number of points.
        ar >> points;

        // Load the PlotPoint data.
        for (UINT u = 0; u < points; ++u)
        {
            ArchiveObject ao( &pp, sizeof(pp) );
            ar >> ao;
            GetAllPoints().push_back(pp);
        }
    }

}

// Stores the specified point information.
void CDoc::StorePoint(int x, int y, bool isPenDown, COLORREF penColor)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;
    pp.penColor = penColor;

    m_points.push_back(pp); // Add the point to the vector.
}