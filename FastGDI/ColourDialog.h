


#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H

#include "../Win32++/dialog.h"
#include "resource.h"


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT nResID) : CDialog(nResID), m_hBitmap(0) {}
    virtual ~CColourDialog();
	virtual void CColourDialog::SetBitmap(HBITMAP hbm);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnPaint();

private:
	CWnd m_RedSlider;
	CWnd m_GreenSlider;
	CWnd m_BlueSlider;

	CWnd m_RedEdit;
	CWnd m_GreenEdit;
	CWnd m_BlueEdit;

	CWnd m_hWndBitmap;
	HBITMAP m_hBitmap;
};


#endif // COLOURDIALOG_H

