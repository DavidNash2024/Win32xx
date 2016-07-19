

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H


// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.

class CDoc : public CObject
{
public:
	CDoc() {}
	~CDoc() {}

	std::vector<PlotPoint>& GetAllPoints() { return m_points; }
	BOOL FileOpen(LPCTSTR szFilename);
	BOOL FileSave(LPCTSTR szFilename);
	void Serialize(CArchive &ar);
	void StorePoint(PlotPoint& pp);

private:
	std::vector<PlotPoint> m_points;	// Points of lines to draw

};

#endif	// WIN32XX_DOC_H