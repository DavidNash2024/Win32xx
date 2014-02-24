//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include <olectl.h>


// Declaration of the CView class
class CView : public CWnd
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
	virtual void    OnDraw(CDC* pDC);
	virtual LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual void    OnInitialUpdate();
	virtual LRESULT OnVScroll(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
	virtual void    PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPPICTURE m_pPicture;
	CBrush	m_Brush;
	OLECHAR m_OleString[MAX_STRING_SIZE];
	BSTR    m_BStrString;
	int m_xCurrentScroll;
	int	m_yCurrentScroll;
};


#endif // VIEW_H
