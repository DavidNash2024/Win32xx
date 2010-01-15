
#pragma once

#include <UIRibbon.h>
#include "Application.h"

class CRibbon
{
public:
	CRibbon(); 
	virtual ~CRibbon();
	bool InitializeRibbon(HWND hWnd);
	void DestroyRibbon();

private:
	IUIFramework* m_pFramework;
	CApplication* m_pApplication;
};
