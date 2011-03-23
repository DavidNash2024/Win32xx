//////////////////////////////////////////////
// ColourDialog.h

#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT nResID);
    virtual ~CColourDialog();
	void CreateImagePreviews(HBITMAP hbmImage);
	
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnGrayScale();
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

