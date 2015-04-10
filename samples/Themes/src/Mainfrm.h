/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "MyCombo.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

	void AddCombo();
	BOOL ChooseColor(UINT nColor);
	DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);
	BOOL OnFileExit();
	BOOL OnUseThemes();
	BOOL OnBandColors();
	BOOL OnFlatStyle();
	BOOL OnLeftBands();
	BOOL OnLockMenuBar();
	BOOL OnRoundBorders();
	BOOL OnShortBands();
	BOOL OnUseLines();
	BOOL OnViewArrows();
	BOOL OnViewCards();
	void SetReBarColors(COLORREF clrBkGnd1, COLORREF clrBkGnd2, COLORREF clrBand1, COLORREF clrBand2);
	void ShowArrows(BOOL bShow);
	void ShowCards(BOOL bShow);

protected:
	virtual LRESULT CustomDrawMenuBar(NMHDR* pNMHDR);
	virtual BOOL LoadRegistrySettings(LPCTSTR szKeyName);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CToolBar m_Arrows;
	CToolBar m_Cards;
	CMyCombo m_ComboBoxEx;
	std::vector<UINT> m_vBandIDs;
	std::vector<UINT> m_vBandStyles;
	std::vector<UINT> m_vBandSizes;
	CImageList m_ArrowImages;
	CImageList m_CardImages;

	UINT m_nColor;
	BOOL m_UseThemes;
	BOOL m_UseBandColors;
	BOOL m_UseFlatStyle;
	BOOL m_KeepBandsLeft;
	BOOL m_LockMenuBand;
	BOOL m_UseRoundBorders;
	BOOL m_UseShortBands;
	BOOL m_UseLines;
	BOOL m_ShowArrows;
	BOOL m_ShowCards;
};

#endif //MAINFRM_H

