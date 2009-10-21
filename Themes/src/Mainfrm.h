/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "ComboBoxEx.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void AddCombo();
	void ChooseColor(UINT nColor);
	void ShowArrows(BOOL bShow);
	void ShowCards(BOOL bShow);

protected:
	virtual void LoadRegistrySettings(LPCTSTR szKeyName);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SaveRegistrySettings();
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);
	void OnUseThemes();
	void OnBandColors();
	void OnFlatStyle();
	void OnLeftBands();
	void OnLockMenubar();
	void OnRoundBorders();
	void OnShortBands();
	void OnUseLines();
	void OnViewArrows();
	void OnViewCards();
	void SetRebarTheme(COLORREF clrBkGnd1, COLORREF clrBkGnd2, COLORREF clrBand1, COLORREF clrBand2);

	CView m_View;
	CToolbar Arrows;
	CToolbar Cards;
	CComboBoxEx m_ComboBoxEx;
	std::vector<UINT> m_vBandIDs;
	std::vector<UINT> m_vBandStyles;
	
	UINT m_nColor;
	BOOL m_bUseThemes;
	BOOL m_bBandColors;
	BOOL m_bFlatStyle;
	BOOL m_bBandsLeft;
	BOOL m_bLockMenuBand;
	BOOL m_bRoundBorders;
	BOOL m_bShortBands;
	BOOL m_bUseLines;
	BOOL m_bShowArrows;
	BOOL m_bShowCards;
};

#endif //MAINFRM_H

