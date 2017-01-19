//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include <olectl.h>


// Declaration of the CView class
class CView : public CScrollView
{
public:
	CView();
	virtual ~CView();
	virtual CRect GetImageRect();
	virtual BOOL LoadPictureFile(LPCTSTR szFile);
	virtual void NewPictureFile();
	virtual void SavePicture(LPCTSTR szFile);
	void Paint(HDC hDC);

	LPPICTURE GetPicture() { return m_pPicture; }

protected:
	virtual void    OnDraw(CDC& dc);
	virtual LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int     OnCreate(CREATESTRUCT& cs);
	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPPICTURE m_pPicture;
	CBrush	m_Brush;
};


#endif // VIEW_H
