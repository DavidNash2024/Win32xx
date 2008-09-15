


#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H

#include "../Win32++/dialog.h"
#include "resource.h"


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT nResID); 
    virtual ~CColourDialog();
	virtual void CColourDialog::CreateImagePreview(HBITMAP hbmImage);
	int GetcRed() {return m_cRed;}
	int GetcGreen() {return m_cGreen;}
	int GetcBlue() {return m_cBlue;}

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnHScroll(WPARAM wParam, LPARAM lParam);
	void OnPaint();

	CWnd m_RedSlider;
	CWnd m_GreenSlider;
	CWnd m_BlueSlider;

	CWnd m_RedEdit;
	CWnd m_GreenEdit;
	CWnd m_BlueEdit;

	CWnd m_hWndBitmap;
	HBITMAP m_hbmPreview;
	HBITMAP m_hbmPreviewOrig;

	int m_cRed;
	int m_cGreen;
	int m_cBlue;
};


#endif // COLOURDIALOG_H

