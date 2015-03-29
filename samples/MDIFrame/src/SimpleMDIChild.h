//////////////////////////////////////////////////////
// SimpleMDIChild.h
//  Declaration of the CSimpleView and CSimpleMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H



// Declaration of CSimpleView
class CSimpleView : public CWnd
{
public:
	CSimpleView();
	virtual void OnDraw(CDC& dc);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	COLORREF GetColor() {return m_Color;}
	void SetColor(COLORREF color) { m_Color = color; }

private:
	COLORREF m_Color;
};


// Declaration of CSimpleMDIChild
class CSimpleMDIChild : public CMDIChild
{
public:
	CSimpleMDIChild();
	virtual ~CSimpleMDIChild();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CSimpleView m_View;
};

#endif  //MDICHILDVIEW_H
