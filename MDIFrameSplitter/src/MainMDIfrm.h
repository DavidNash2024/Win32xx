///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H



// MDI frames are inherited from CMDIFrame
class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnFileOpen();
	virtual	void OnFilePrint();
	virtual void OnInitialUpdate();
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

