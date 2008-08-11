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

#define DN_DOCK_START		WM_APP + 1	
#define DN_DOCK_MOVE		WM_APP + 2	
#define DN_DOCK_END			WM_APP + 3	
#define DN_BAR_START		WM_APP + 4	
#define DN_BAR_MOVE			WM_APP + 5	
#define DN_BAR_END			WM_APP + 6  

// Docking Messages
#define DN_CANDOCKHERE		WM_APP + 7
#define DN_FOCUSCHANGE      WM_APP + 8

namespace Win32xx
{

	typedef struct DRAGPOS
	{
		NMHDR hdr;
		POINT ptPos;
	} *LPDRAGPOS;

	class CDockable;

	class CDockBar : public CWnd
	{
	public:
		CDockBar() : m_IsCaptured(FALSE), m_pDockable(NULL) {m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));}
		~CDockBar() {::DeleteObject(m_hbrBackground);}

		virtual void PreCreate(CREATESTRUCT& cs)
		{
		//	cs.dwExStyle = WS_EX_CLIENTEDGE;
		}
		
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

	class CDockCaption : public CWnd
	{
	public:
		CDockCaption() {}
		virtual ~CDockCaption() {}
		virtual void OnPaint(HDC hDC);
		virtual void SendNotify(UINT nMessageID);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
		virtual void DrawCaption();
		virtual void DrawHashBar(HWND hBar, POINT Pos);
		virtual UINT GetDockSide(LPDRAGPOS pdp);
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual void RecalcDockLayout();
		virtual void SendNotify(UINT nMessageID);
		virtual void UnDock();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Attributes
		BOOL IsDocked() {return (BOOL)m_DockState;}
	//	CDockable* GetDockParent() {return m_pDockParent;}
		CRect& GetDockRect() {return m_rcDock;}
		UINT GetDockState() {return m_DockState;}
		int GetDockWidth() {return m_DockWidth;}
		void SetDockRect(RECT rc) {m_rcDock = rc;}
		void SetDockState(UINT uDockState) {m_DockState = uDockState;}
		void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}

	public:
		UINT m_DockState;
		CDockBar m_Bar;
		CDockCaption m_Caption;
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
	//	HWND m_hDockParent;

	}; // class CDockable

} // namespace Win32xx

#endif // DOCKABLE_H

