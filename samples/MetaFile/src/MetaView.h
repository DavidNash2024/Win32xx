///////////////////////////////////////
// MetaView.h


#ifndef METAVIEW_H
#define METAVIEW_H


#include "wxx_wincore.h"


// A class that inherits from CWnd. It is used to create the window.
class CMetaView : public CWnd
{
public:
	CMetaView() {}
    virtual ~CMetaView() {}
	virtual int OnCreate(CREATESTRUCT&);
	virtual void OnDraw(CDC& dc);
    virtual void OnDestroy();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CEnhMetaFile  m_EnhMetaFile;

};

#endif // METAVIEW_H