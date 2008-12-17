//////////////////////////////////////////////////////
// container.h


#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include "../Win32++/wincore.h"
#include "../Win32++/toolbar.h"

class CContainer;

struct TabPageInfo
{
//	CWnd* pwndView;
	TCHAR szTitle[MAX_MENU_STRING];
	HICON hIcon;
	CContainer* pwndContainer;
};

////////////////////////////////////
// Declaration of the CTabPage class
class CTabPage : public CWnd
{
public:
	CTabPage()  {}
	virtual ~CTabPage() {}
	virtual CToolbar& GetToolbar() const		{return (CToolbar&)m_wndToolbar;}
	virtual CWnd* GetView() const		{return m_pwndView;}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual void RecalcLayout();
	virtual void SetView(CWnd& wndView);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CToolbar m_wndToolbar;
	CWnd* m_pwndView;

};

///////////////////////////////////////
// Declaration of the CContainer class
class CContainer : public CWnd
{
public:
	CContainer();
	virtual ~CContainer();
	virtual void AddContainer(CContainer* pwndContainer, LPCTSTR szTitle, HICON hIcon);
	virtual void AddToolbarButton(UINT nID);
	virtual int FindPage(CWnd* pwndPage);
	virtual SIZE GetMaxTabTextSize();
	virtual CTabPage& GetPage() const		{return (CTabPage&)m_wndPage;}
	virtual CToolbar& GetToolbar() const	{return m_wndPage.GetToolbar();}	
	virtual void RemoveContainer(CContainer& Wnd);
	virtual void SelectPage(int iPage);

protected:
	virtual void OnCreate();
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HIMAGELIST m_himlTab;
	std::vector<UINT> m_vToolbarData;	
	std::vector<TabPageInfo> m_vTabPageInfo;
	int m_iCurrentPage;
	CTabPage m_wndPage; 
};


#endif // CONTAINER_H
