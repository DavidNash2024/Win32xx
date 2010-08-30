//////////////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


// Declaration of the CMyCombo class
class CMyCombo : public CWnd
{
public:
	CMyCombo();
	virtual ~CMyCombo();
	BOOL AddItems();
	void SetImages(int nImages, UINT ImageID);

protected:
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HIMAGELIST m_himlImages;
};


#endif // MYCOMBO_H
