/////////////////////////////
// MetaView.h
//

#ifndef METAVIEW_H
#define METAVIEW_H


#include "wxx_wincore.h"


////////////////////////////////////////////////////////////
// CMetaView manages the application's main window.
// An enhanced meta file is drawn in a pattern on the window.
class CMetaView : public CWnd
{
public:
    CMetaView() {}
    virtual ~CMetaView() {}

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT&);
    virtual void OnDraw(CDC& dc);
    virtual void OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CEnhMetaFile  m_enhMetaFile;
};

#endif // METAVIEW_H