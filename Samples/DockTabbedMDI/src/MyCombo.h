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
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HIMAGELIST m_himlImages;
};


#endif // MYCOMBO_H
