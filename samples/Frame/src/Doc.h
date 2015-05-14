

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H

#include "View.h"


class CDoc : public CObject
{
public:
	CDoc() {}
	~CDoc() {}

	BOOL FileLoad(LPCTSTR szFilename);
	BOOL FileStore(LPCTSTR szFilename);
	void Serialize(CArchive& ar);

private:
//	int m_MemberVariable;

};

#endif	// WIN32XX_DOC_H