///////////////////////////////////////
// SdiView.h

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "dialog.h"
#include "resource.h"


// Declaration of the CFormView class
class CFormView : public CDialog
{
public:
  CFormView(UINT nResID, HWND hParent = NULL);
  virtual ~CFormView();
  virtual HWND Create(HWND hParent);

  void OnRangeOfIds_Radio(UINT nIdAdjust);
  void OnCheckA();
  void OnCheckB();
  void OnCheckC();

protected:
  virtual void OnCancel();
  virtual BOOL OnInitDialog();
  virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  virtual void OnOK();

private:
  void OnButton();

  HMODULE m_hInstRichEdit;
};

#endif //SDI_VIEW_H
