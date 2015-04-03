//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CViewMax and CMDIChildMax classes

#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H


// Declaration of CViewMax
class CViewMax : public CWnd
{
public:
	CViewMax() {}
	virtual ~CViewMax() {}

protected:
	virtual void OnDraw(CDC& dc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// Declaration of CMDIChildMax
class CMDIChildMax : public CMDIChild
{
public:
	CMDIChildMax();
	virtual ~CMDIChildMax();

protected:
	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	CViewMax m_MaxView;
	CMenu m_Menu;
};

#endif  //MDICHILDMAX_H
