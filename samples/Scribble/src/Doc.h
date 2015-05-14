

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H

#include "View.h"


class CDoc : public CObject
{
public:
	CDoc() {}
	~CDoc() {}

	std::vector<PlotPoint>& GetPoints() {return m_points;}	// returns a vector of PlotPoint data
	
	BOOL FileOpen(LPCTSTR szFilename);
	BOOL FileSave(LPCTSTR szFilename);
	void Serialize(CArchive &ar);
	void StorePoint(int x, int y, bool PenDown, COLORREF PenColor);

private:
	std::vector<PlotPoint> m_points;	// Points of lines to draw

};

#endif	// WIN32XX_DOC_H