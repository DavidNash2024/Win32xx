/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


///////////////////////////////////////////////
// CBalloonToolTip manages a tooltip which uses
// a balloon style.
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


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog();

protected:
    // Virtual functions that override base class functions 
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID);

    // Member variables
    CRichEdit m_richEdit;
    CEdit m_edit;
    CBalloonToolTip m_bubbleTT;
    CToolTip m_toolTip;
    CString m_str;
    HICON m_info;      // Icon used by tooltips for the RichEdit and Edit controls
};


#endif //MYDIALOG_H
