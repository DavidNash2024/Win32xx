//////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CMDIChildText class


#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H

#include "../Win32++/MDI.h"

class CRichEdit : public CWnd
{
public:
	CRichEdit();
	virtual ~CRichEdit();
	virtual void OnCreate();
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HFONT m_hFont;
	HMODULE   m_hRichEdit;
};




class CMDIChildText : public CMDIChild
{
public:
	CMDIChildText();
	virtual ~CMDIChildText();

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CRichEdit m_TextWindow;

};

#endif  //MDICHILDTEXT_H
