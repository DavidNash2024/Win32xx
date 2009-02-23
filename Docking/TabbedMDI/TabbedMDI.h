////////////////////////////////////////////
// TabbedMDI.h - Declaration of the CTabbedMDI 
//            and CDockTabbedMDI classes


#ifndef TABBEDMDI_H
#define TABBEDMDI_H

#include "../../Win32++/tab.h"
#include "../../Win32++/docking.h"


// Declaration of the CTabbedMDI class
class CTabbedMDI : public CWnd
{
public:
	CTabbedMDI() {}
	virtual ~CTabbedMDI() {}
	virtual CWnd* AddMDIChild(CWnd* pWnd, LPCTSTR szTabText);

protected:
	virtual HWND Create(HWND hWndParent);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTab m_Tab;
};


// Declaration of the CDockTabbedMDI class
class CDockTabbedMDI : public CDockable
{
public:
	CDockTabbedMDI();
	virtual ~CDockTabbedMDI() {}
	virtual void OnInitialUpdate();

private:
	CTabbedMDI m_View;

};

#endif // TABBEDMDI_H
