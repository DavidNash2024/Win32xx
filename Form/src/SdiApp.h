//////////////////////////////////////////////////
// SdiApp.h

#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"

// Declaration of the CFrameApp class
class CSdiApp : public CWinApp
{
public:
  CSdiApp();
  virtual ~CSdiApp();
  virtual BOOL InitInstance();
  CMainFrame& GetMainFrame() { return m_Frame; }

private:
  CMainFrame m_Frame;

};


// Handy global functions
inline CSdiApp& GetSdiApp() { return *((CSdiApp*)GetApp()); }


#endif // define SDI_APP_H

