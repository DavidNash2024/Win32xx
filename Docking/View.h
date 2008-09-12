//////////////////////////////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/wincore.h"


// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void PreCreate(CREATESTRUCT &cs);

};


#endif // VIEW_H
