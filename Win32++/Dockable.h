///////////////////////////////////////////////////////
// Dockable.h
//  Declaration of the CDockable class

#ifndef DOCKABLE_H
#define DOCKABLE_H


#include "../Win32++/WinCore.h"
#include "../Win32++/GDI.h"


// Docking States
#define DS_DOCKED_LEFT		0x0001
#define DS_DOCKED_RIGHT		0x0002
#define DS_DOCKED_TOP		0x0004
#define DS_DOCKED_BOTTOM	0x0008

namespace Win32xx
{

enum Constants1
{
	// Docking Notifications
	DN_DOCK_START= WM_APP + 1,	// Start dock window drag
	DN_DOCK_MOVE = WM_APP + 2,	// Dock window draging
	DN_DOCK_END  = WM_APP + 3,	// End dock window dragging
	DN_BAR_START = WM_APP + 4,	// Start splitter bar drag
	DN_BAR_MOVE  = WM_APP + 5,	// Splitter bar dragging
	DN_BAR_END   = WM_APP + 6,  // End splitter bar drag

	// Docking Messages
	DN_CANDOCKHERE = WM_APP + 7   // Over dockable area of CDockable child  
};

typedef struct DRAGPOS
{
    NMHDR hdr;
    POINT ptPos;
} *LPDRAGPOS;

class CDockable;

class CBar : public CWnd
{
public:
	CBar() : m_IsCaptured(FALSE), m_pDockable(NULL) {m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));}
	~CBar() {::DeleteObject(m_hbrBackground);}

	virtual void PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ Bar");
		wc.hbrBackground = m_hbrBackground;
	}

	void SendNotify(UINT nMessageID);

	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	CDockable* m_pDockable;

private:
	HBRUSH m_hbrBackground;
	BOOL m_IsCaptured;
	
};

class CDockable : public CWnd
{
public:
	// Operations
	CDockable();
	virtual ~CDockable();
	virtual CDockable* AddDockChild(CDockable* pDockable, UINT uDockSide, int DockWidth);
	virtual void Dock(CDockable* hDockable, UINT uDockSide);
	virtual void Draw3DBorder(RECT& Rect);
	virtual void DrawHashBar(HWND hBar, POINT Pos);
	virtual UINT GetDockSide(LPDRAGPOS pdp);
	virtual void OnCreate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual void RecalcDockLayout(HDWP& hdwp);
	virtual void SendNotify(UINT nMessageID);
	virtual void UnDock();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Attributes
	BOOL IsDocked() {return (BOOL)m_DockState;}
	CDockable* GetDockParent() {return m_pDockParent;}
	CRect& GetDockRect() {return m_rcDock;}
	UINT GetDockState() {return m_DockState;}
	int GetDockWidth() {return m_DockWidth;}
	void SetDockRect(RECT rc) {m_rcDock = rc;}
	void SetDockState(UINT uDockState) {m_DockState = uDockState;}
	void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}

public:
	UINT m_DockState;
	CBar m_Bar;
	CDockable* m_pDockParent;
	std::vector <CDockable*> m_vDockChildren;
	int m_DockWidth;
	int m_NCHeight;
	CRect m_rcDock;
	CWnd* m_pView;
	int m_BarWidth;
	HBRUSH m_hbrDithered;
	HBITMAP	m_hbm;
	BOOL m_IsDraggingDockable;
	BOOL m_IsInDockZone;
	HWND m_hDockParent;
};

}

#endif // DOCKABLE_H

