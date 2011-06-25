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
	virtual void OnPaint(CDC* pDC);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	COLORREF GetColor() {return m_Color;}
	void SetColor(COLORREF color) { m_Color = color; }

private:
	COLORREF m_Color;
};

// Declaration of the CDockSimple class
class CDockSimple : public CDocker
{
public:
	CDockSimple() {SetView(m_View);}
	virtual ~CDockSimple() {}

protected:
//	virtual CDocker* NewDockerFromID(int nID);
//	virtual void OnInitialUpdate();

private:
	CSimpleView m_View;

};

// Declaration of CSimpleMDIChild
class CSimpleMDIChild : public CMDIChild
{
public:
	CSimpleMDIChild();
	virtual ~CSimpleMDIChild();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDockSimple m_View;
};

#endif  //MDICHILDVIEW_H
