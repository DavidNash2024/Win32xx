///////////////////////////////////////////////////////
// DockFrame.h
//  Declaration of the CDockFrame class

#ifndef DOCKFRAME_H
#define DOCKFRAME_H


#include "../Win32++/MDI.h"
#include "Dockable.h"


// Docking Styles
#define DS_DOCK_LEFT		0x0001
#define DS_DOCK_RIGHT		0x0002
#define DS_DOCK_TOP			0x0004
#define DS_DOCK_BOTTOM		0x0008


class CDockFrame : public CMDIFrame
{
public:
	CDockFrame(void);
	virtual ~CDockFrame();
	void AddDockable(CDockable* pDockable, UINT uDockSide, int DockWidth);
	UINT GetDockSide(LPDRAGPOS pdp);
	virtual void Dock(HWND hDockable, UINT DockState);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void RecalcLayout();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class CBar : public CWnd
	{
	public:
		CBar()  {m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));}
		~CBar() {::DeleteObject(m_hbrBackground);}

		virtual void PreRegisterClass(WNDCLASS& wc)
		{
			wc.lpszClassName = _T("Win32++ Bar");
			wc.hCursor = ::LoadCursor (NULL, IDC_SIZEWE);
			wc.hbrBackground = m_hbrBackground;
		}

	private:
		HBRUSH m_hbrBackground;
	};

	std::vector<CDockable*> m_vDockables;
	std::vector<CBar*> m_vBars;
};


#endif  // DOCKFRAME_H

