
#include "stdafx.h"
#include "ScribbleApp.h"
#include "Doc.h"


BOOL CDoc::FileOpen(LPCTSTR szFilename)
{
	GetAllPoints().clear();
	BOOL bResult = FALSE;

	try
	{
		CArchive ar(szFilename, CArchive::load);
		ar >> *this;
		bResult = TRUE;
	}

	catch (const CFileException &e)
	{
		// An exception occurred. Display the relevant information.
		::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
		
		GetAllPoints().clear();
	}

	return bResult;
}

BOOL CDoc::FileSave(LPCTSTR szFilename)
{
	BOOL bResult = TRUE;

	try
	{
		CArchive ar(szFilename, CArchive::store);
		ar << *this;
	}
	catch (const CFileException &e)
	{
		// An exception occurred. Display the relevant information.
		::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
		bResult = FALSE;
	}

	return bResult;
}

void CDoc::Serialize(CArchive &ar)
// Uses CArchive to stream data to or from a file
{

	if (ar.IsStoring())
	{
		// Store the number of points
		UINT nPoints = GetAllPoints().size();
		ar << nPoints;

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
		UINT nPoints;
		PlotPoint pp;
		GetAllPoints().clear();

		// Load the number of points
		ar >> nPoints;

		// Load the PlotPoint data
		for (UINT u = 0; u < nPoints; ++u)
		{
			ArchiveObject ao(&pp, sizeof(pp));
			ar >> ao;
			GetAllPoints().push_back(pp);
		}
	}

}

void CDoc::StorePoint(PlotPoint& pp)
{
	m_points.push_back(pp); //Add the point to the vector
}