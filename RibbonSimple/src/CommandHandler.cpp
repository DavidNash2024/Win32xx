///////////////////////////////
// CommandHandler.cpp

#include <wincore.h>
#include <UIRibbon.h>
#include "CommandHandler.h"
#include "Ribbon.h"
#include "SimpleRibbonUI.h"

// Static method to create an instance of the object.
__checkReturn HRESULT CCommandHandler::CreateInstance(__deref_out IUICommandHandler **ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = NULL;

    HRESULT hr = S_OK;
   
    CCommandHandler* pCommandHandler = new CCommandHandler();

    if (pCommandHandler != NULL)
    {
        *ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler))
    {
        *ppv = static_cast<IUICommandHandler*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
    UINT nCmdID,
    __in REFPROPERTYKEY key,
    __in_opt const PROPVARIANT* ppropvarCurrentValue,
    __out PROPVARIANT* ppropvarNewValue)
{
    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(ppropvarCurrentValue);
    UNREFERENCED_PARAMETER(ppropvarNewValue);

    return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    __in_opt const PROPERTYKEY* key,
    __in_opt const PROPVARIANT* ppropvarValue,
    __in_opt IUISimplePropertySet* pCommandExecutionProperties)
{
    UNREFERENCED_PARAMETER(pCommandExecutionProperties);
    UNREFERENCED_PARAMETER(ppropvarValue);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(verb);

    HRESULT hr = S_OK;
	switch(nCmdID)
	{
	case cmdButton1:
		TRACE(_T("Button 1\n"));
		break;
	case cmdButton2:
		TRACE(_T("Button 2\n"));
		break;
	case cmdButton3:
		TRACE(_T("Button 3\n"));
		break;
	case cmdButton4:
		TRACE(_T("Button 4\n"));
		break;
	case cmdButton5:
		TRACE(_T("Button 5\n"));
		break;
	case cmdButton6:
		TRACE(_T("Button 6\n"));
		break;
	case cmdToggleButton1:
		TRACE(_T("Toggle Button 1\n"));
		break;
	case cmdToggleButton2:
		TRACE(_T("Toggle Button 2\n"));
		break;
	case IDC_CMD_EXIT:
		TRACE(_T("Exit button\n"));
		break;
	default:
		TRACE(_T("Unknown button\n"));
		break;
	}

	return hr;
}
