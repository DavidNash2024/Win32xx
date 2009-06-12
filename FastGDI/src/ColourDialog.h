


#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H

#include "../../Win32++/dialog.h"
#include "resource.h"


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT nResID);
    virtual ~CColourDialog();
	virtual void CColourDialog::CreateImagePreviews(HBITMAP hbmImage);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnHScroll(WPARAM wParam, LPARAM lParam);
	void OnPaintPreview();

	// CWnd object for the dialog controls to attach to
	CWnd m_RedSlider;
	CWnd m_GreenSlider;
	CWnd m_BlueSlider;
	CWnd m_RedEdit;
	CWnd m_GreenEdit;
	CWnd m_BlueEdit;
	CWnd m_Preview;

	HBITMAP m_hbmPreview;
	HBITMAP m_hbmPreviewOrig;
	int m_cRed;
	int m_cGreen;
	int m_cBlue;
};


#endif // COLOURDIALOG_H

