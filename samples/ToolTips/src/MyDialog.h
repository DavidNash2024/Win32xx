/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H

////////////////////////////////////////
// CBalloonToolTip function definitions.
//
class CBalloonToolTip : public CToolTip
{
public:
    CBalloonToolTip() {}
    virtual ~CBalloonToolTip() {}

protected:
    virtual void PreCreate(CREATESTRUCT& cs)
    {
        CToolTip::PreCreate(cs);
        cs.style |= TTS_BALLOON;    // Add the balloon style
    }

};


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog();

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID);

    CRichEdit m_richEdit;
    CEdit m_edit;
    CBalloonToolTip m_bubbleTT;
    CToolTip m_toolTip;
    CString m_str;
    HICON m_info;      // Icon used by tooltips for the RichEdit and Edit controls
};

#endif //MYDIALOG_H
