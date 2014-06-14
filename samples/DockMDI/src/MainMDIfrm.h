///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H

enum DockIDs
{
	ID_DOCK_CLASSES = 1,
	ID_DOCK_FILES = 2,
	ID_DOCK_OUTPUT = 3,
	ID_DOCK_TEXT = 4,
	ID_DOCK_BROWSER = 5,
	ID_DOCK_RECT = 6,
	ID_DOCK_DIALOG = 7
};

class CDockMDIClient : public CDocker::CDockClient
{
public:
	CDockMDIClient() {}
	virtual ~CDockMDIClient() {}
	CMDIFrame* GetMDIFrame() const { return static_cast<CMDIFrame*>(GetParent()); }

protected:
	HWND Create(CWnd* pParent);
	LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
	LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
	LRESULT OnMDISetMenu(WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


// MDI frames are inherrited from CMDIDockFrame
class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

	virtual CWnd* GetMDIClient() const { return (CWnd*)(GetDockClient()); }
	virtual CDockClient* GetDockClient() const	{ return (CDockClient*)(&m_DockMDIClient); }

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnFileClose();
	virtual void OnFileExit();
	virtual void OnFileOpen();
	virtual void OnFileSave();
	virtual	void OnFilePrint();
	virtual void OnInitialUpdate();
	virtual void RecalcLayout();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDockMDIClient m_DockMDIClient;
};

#endif  //MAINMDIFRM_H

