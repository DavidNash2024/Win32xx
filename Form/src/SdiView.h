///////////////////////////////////////
// SdiView.h

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "dialog.h"
#include "resource.h"


// Declaration of the CSdiView class
class CSdiView : public CDialog
{
public:
  CSdiView(UINT nResID, HWND hParent = NULL);
  virtual ~CSdiView();
  virtual HWND Create(HWND hParent);

  void OnRangeOfIds_Radio(UINT nIdAdjust);
  void OnCheckA();
  void OnCheckB();
  void OnCheckC();

protected:
  virtual void OnCancel();
  virtual BOOL OnInitDialog();
  virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  virtual void OnOK();

private:
  void OnButton();

  HMODULE m_hInstRichEdit;
};

#endif //SDI_VIEW_H
