
#include "MyPropertySheet.h"
#include "Mainfrm.h"
#include "resource.h"


CButtonPage::CButtonPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

BOOL CButtonPage::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR     lpnmhdr;

	switch (uMsg)
	{

	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		TRACE("WM_COMMAND in CButtonPage::DialogProc");
		if (LOWORD(wParam) == IDOK)
			TRACE("Got OK button in CButtonPage::DialogProc");
		PropSheet_Changed(GetParent(hWnd), hWnd);
		break;

	case WM_DESTROY:
		TRACE("Destroyed");
		break;

	case WM_NOTIFY:
		lpnmhdr = (LPNMHDR)lParam;

		switch (lpnmhdr->code)
			{
			case PSN_APPLY:   //sent when OK or Apply button pressed
				TRACE("Apply or OK button pressed");
				break;

			case PSN_RESET:   //sent when Cancel button pressed
				TRACE("Cancel button pressed");
				break;

			case PSN_SETACTIVE:
				//this will be ignored if the property sheet is not a wizard
				PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_NEXT);
				break;

			default:
				break;
			}
		break;

	default:
		break;
	}
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

CComboPage::CComboPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

BOOL CComboPage::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR     lpnmhdr;

	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		PropSheet_Changed(GetParent(hWnd), hWnd);
		TRACE("WM_COMMAND");
		break;

	case WM_INITDIALOG:
			TRACE("WM_INITDIALOG ");
			break;

	case WM_NOTIFY:
		lpnmhdr = (LPNMHDR)lParam;

		switch (lpnmhdr->code)
			{
			case PSN_APPLY:   //sent when OK or Apply button pressed
				break;

			case PSN_RESET:   //sent when Cancel button pressed
				break;

			case PSN_SETACTIVE:
				//this will be ignored if the property sheet is not a wizard
				PropSheet_SetWizButtons(GetParent(hWnd), PSWIZB_BACK | PSWIZB_FINISH);
				return FALSE;

			default:
				break;
			}
		break;

	default:
	break;
   }

	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption /*=NULL*/, HWND hwndParent /* = NULL*/) : CPropertySheet(pszCaption, hwndParent)
{
	m_PSH.pszIcon          = MAKEINTRESOURCE(IDI_BACKCOLOR);
	m_PSH.dwFlags          = PSH_PROPSHEETPAGE | PSH_USEICONID  | PSH_USECALLBACK;
}

BOOL CMyPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	TRACE("CMyPropertySheet::OnCommand");

    UINT nID = LOWORD(wParam);
    HWND hWndCtrl = (HWND)lParam;
    int nCode = HIWORD(wParam);
    
    // set m_nModalResult to ID of button, whenever button is clicked
    if (hWndCtrl != NULL && nCode == BN_CLICKED)
    {
        if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0) &
            (DLGC_BUTTON|DLGC_DEFPUSHBUTTON))
        {
       //     LONG lStyle = ::GetWindowLong(hWndCtrl, GWL_STYLE) & 0x0F;
       //     if (lStyle == BS_PUSHBUTTON || lStyle == BS_DEFPUSHBUTTON ||
       //         lStyle == BS_USERBUTTON || lStyle == BS_OWNERDRAW)
            {
                if (nID == IDOK)
                {
					TRACE("OK Button Hit");
           /*         if (YOU_WANT_TO_CLOSE_THE_PROPERTY_SHEET)
                    {
                        // do whatever you want before closing the property page

                        // You dont have to assign nID to 
                        // m_nModalResult. If you want to 
                        // return IDOK or IDCANCEL instead of 
                        // the default return value, 
                        // you can do it by assigning IDOK or 
                        // IDCANCEL to m_nModalResult
                        m_nModalResult = nID;
                    }
                    else
                    {
                        // do whatever you want.
                        return TRUE;
                    } */
                }
                else if (nID == ID_APPLY_NOW)
                {
					TRACE("Apply button Hit");
                /*    if (YOU_WANT_TO_CLOSE_THE_PROPERTY_SHEET)
                    {
                        // do whatever you want before 
                        // closing the property page
                        
                        // You dont have to assign nID to 
                        // m_nModalResult. If you want to 
                        // return IDOK or IDCANCEL instead 
                        // of the default return value, 
                        // you can do it by assigning IDOK or 
                        // IDCANCEL to m_nModalResult
  //                      m_nModalResult = nID;
                    }
                    else
                    {
                        // do whatever you want.
                        return TRUE;
                    }*/
                } 
                else if (nID == IDCANCEL)
                {
					TRACE("Cancel Button Hit");
           /*         if (YOU_WANT_TO_CLOSE_THE_PROPERTY_SHEET)
                    {
                        // do whatever you want before 
                        // closing the property page

                        // You dont have to assign nID to 
                        // m_nModalResult. If you want to 
                        // return IDOK or IDCANCEL instead 
                        // of the default return value, 
                        // you can do it by assigning IDOK or 
                        // IDCANCEL to m_nModalResult
                        m_nModalResult = nID;
                    }
                    else
                    {
                        // do whatever you want.
                        return TRUE;
                    } */
                }
            }
        }
    }
//   return FALSE;
	return TRUE;
}

LRESULT CMyPropertySheet::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);	
}

