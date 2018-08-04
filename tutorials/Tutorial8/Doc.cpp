
#include "Doc.h"

BOOL CDoc::FileOpen(LPCTSTR filename)
{
    GetAllPoints().clear();
    BOOL isFileOpened = FALSE;

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;
        isFileOpened = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);

        GetAllPoints().clear();
    }

    return isFileOpened;
}

BOOL CDoc::FileSave(LPCTSTR filename)
{
    BOOL isFileSaved = FALSE;

    try
    {
        CArchive ar(filename, CArchive::store);
        ar << *this;
        isFileSaved = TRUE;
    }
    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
    }

    return isFileSaved;
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
            ArchiveObject ao( &(*iter), sizeof(PlotPoint) );
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

void CDoc::StorePoint(int x, int y, bool isPenDown, COLORREF penColor)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;
    pp.penColor = penColor;

    m_points.push_back(pp); //Add the point to the vector
}
