////////////////////////////////////////////////////
// Mainfrm.h


#ifndef MAINFRM_H
#define MAINFRM_H


#include "../WinDev++/frame.h"
#include <string>
#include <sstream>
#include "RichView.h"

// required for Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
#define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif

typedef std::basic_stringstream<TCHAR> tStringStream;

DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual void OnEditCut();
	virtual void OnEditCopy();
	virtual void OnEditPaste();
	virtual void OnEditDelete();
	virtual void OnEditRedo();
	virtual void OnEditUndo();
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnFileSaveAs();
	virtual void OnFileSave();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual BOOL ReadFile(LPCTSTR szFileName);
	virtual void SetFileName(TCHAR* szFullFileName);
	virtual void SetWindowTitle();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL WriteFile(LPCTSTR szFileName);
	static  DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static  DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

private:
	CRichView m_RichView;
	tString m_stPathName;
};

#endif //MAINFRM_H

