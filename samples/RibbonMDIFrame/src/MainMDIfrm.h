///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainMDIFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


#include "wxx_ribbon.h"
#include "Doc.h"


// Declaration of the CMainMDIFrame class
class CMainMDIFrame : public CRibbonMDIFrame
{
public:
	CMainMDIFrame();
	virtual ~CMainMDIFrame();

	COLORREF GetColorFromPicker() const;
	CDoc& GetDoc() { return m_Doc; }

	void OnFileExit();
	void OnFileNew();
	void OnMDIClose();
	void OnMDIMaximize();
	void OnMDIMaximized(BOOL IsMax);
	void OnMDIMinimize();
	void OnMDIRestore();
	void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
	void SetPenColor(COLORREF clr);

protected:
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
	virtual STDMETHODIMP UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(CREATESTRUCT &cs);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDoc m_Doc;
	CString m_PathName;

};

#endif //MAINMDIFRM_H

