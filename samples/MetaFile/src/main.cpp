//////////////////////////////////
// main.cpp


#include "wxx_wincore.h"
#include "MetaView.h"


class CApp : public CWinApp
{
public:
	CApp() {};
	virtual ~CApp() {}
	virtual BOOL InitInstance()
	{
		m_MetaView.Create();	// throws a CWinException on failure  
		return TRUE;
	}

private:
	CMetaView m_MetaView;
};



int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		CApp theApp;

		return theApp.Run();
	}
	
	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return -1;
	}
}
