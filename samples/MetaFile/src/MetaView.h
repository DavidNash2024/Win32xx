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
    CMetaView() = default;
    virtual ~CMetaView() override = default;

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT&) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void OnDestroy() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMetaView(const CMetaView&) = delete;
    CMetaView& operator=(const CMetaView&) = delete;

    CEnhMetaFile  m_enhMetaFile;
};

#endif // METAVIEW_H