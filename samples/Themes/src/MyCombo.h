//////////////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


// Declaration of the CMyCombo class
class CMyCombo : public CComboBoxEx
{
public:
	CMyCombo();
	virtual ~CMyCombo();
	BOOL AddItems();
	void SetImages(int nImages, UINT ImageID);

protected:
	virtual void PreCreate(CREATESTRUCT& cs);

private:
	CImageList m_imlImages;
};


#endif // MYCOMBO_H
