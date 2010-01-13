
#pragma once

#include <UIRibbon.h>

class CRibbon
{
public:
	CRibbon() {}
	virtual ~CRibbon() {}
	bool InitializeFramework(HWND hWnd);
	void DestroyFramework();

private:
	IUIFramework* m_pFramework;
	IUIApplication* m_pApplication;
};
