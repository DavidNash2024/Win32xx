#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include "Ribbon.h"

namespace Win32xx
{

	// Static method to create an instance of the object.
	__checkReturn HRESULT CRibbonApplication::CreateInstance(__deref_out IUIApplication **ppApplication)
	{
		if (!ppApplication)
		{
			return E_POINTER;
		}

		*ppApplication = NULL;

		HRESULT hr = S_OK;
	   
		CRibbonApplication* pApplication = new CRibbonApplication();

		if (pApplication != NULL)
		{
			*ppApplication = static_cast<IUIApplication *>(pApplication);
		}
		else
		{
			hr = E_OUTOFMEMORY;
		}

		return hr;
	}

	// IUnknown method implementations.
	STDMETHODIMP_(ULONG) CRibbonApplication::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) CRibbonApplication::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}

		return cRef;
	}

	STDMETHODIMP CRibbonApplication::QueryInterface(REFIID iid, void** ppv)
	{
		if (iid == __uuidof(IUnknown))
		{
			*ppv = static_cast<IUnknown*>(static_cast<IUIApplication*>(this));
		}
		else if (iid == __uuidof(IUIApplication))
		{
			*ppv = static_cast<IUIApplication*>(this);
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
	//  FUNCTION: OnCreateUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler)
	//
	//  PURPOSE: Called by the Ribbon framework for each command specified in markup, to allow
	//           the host application to bind a command handler to that command.
	//
	//  COMMENTS:
	//
	//    In this SimpleRibbon sample, the same command handler is returned for all commands
	//    specified in the SimpleRibbon.xml file.
	//    

	STDMETHODIMP CRibbonApplication::OnCreateUICommand(UINT nCmdID, __in UI_COMMANDTYPE typeID, 
												 __deref_out IUICommandHandler** ppCommandHandler)
	{
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(nCmdID);

		if (NULL == m_pCommandHandler)
		{
			HRESULT hr = CRibbonCommandHandler::CreateInstance((IUICommandHandler**)&m_pCommandHandler);
			if (FAILED(hr))
			{
				return hr;
			}
			
			m_pCommandHandler->SetFrame(m_pFrame);
		}

		return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
	}

	//
	//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
	//
	//  PURPOSE: Called when the state of a View (Ribbon is a view) changes, for example, created, destroyed, or resized.
	//
	//
	STDMETHODIMP CRibbonApplication::OnViewChanged(UINT viewId, __in UI_VIEWTYPE typeId, __in IUnknown* pView, 
											 UI_VIEWVERB verb, INT uReasonCode)
	{
		UNREFERENCED_PARAMETER(uReasonCode);
		UNREFERENCED_PARAMETER(viewId);

		HRESULT hr = E_NOTIMPL;

		// Checks to see if the view that was changed was a Ribbon view.
		if (UI_VIEWTYPE_RIBBON == typeId)
		{
			switch (verb)
			{           
				// The view was newly created.
			case UI_VIEWVERB_CREATE:
				hr = S_OK;
				break;

				// The view has been resized.  For the Ribbon view, the application should
				// call GetHeight to determine the height of the ribbon.
			case UI_VIEWVERB_SIZE:
				{
					IUIRibbon* pRibbon = NULL;
					UINT uRibbonHeight;

					hr = pView->QueryInterface(IID_PPV_ARGS(&pRibbon));
					if (SUCCEEDED(hr))
					{
						// Call to the framework to determine the desired height of the Ribbon.
						hr = pRibbon->GetHeight(&uRibbonHeight);
						pRibbon->Release();
						// Use the ribbon height to position controls in the client area of the window.
					}
				}
				break;
				// The view was destroyed.
			case UI_VIEWVERB_DESTROY:
				hr = S_OK;
				break;
			}
		}

		return hr;
	}


	//
	//  FUNCTION: OnDestroyUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler*)
	//
	//  PURPOSE: Called by the Ribbon framework for each command at the time of ribbon destruction.
	//
	STDMETHODIMP CRibbonApplication::OnDestroyUICommand(UINT32 nCmdID, __in UI_COMMANDTYPE typeID,
												  __in_opt IUICommandHandler* commandHandler)
	{
		UNREFERENCED_PARAMETER(commandHandler);
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(nCmdID);

		return E_NOTIMPL;
	}


	// Static method to create an instance of the object.
	__checkReturn HRESULT CRibbonCommandHandler::CreateInstance(__deref_out IUICommandHandler **ppCommandHandler)
	{
		if (!ppCommandHandler)
		{
			return E_POINTER;
		}

		*ppCommandHandler = NULL;

		HRESULT hr = S_OK;
	   
		CRibbonCommandHandler* pCommandHandler = new CRibbonCommandHandler();

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
	STDMETHODIMP_(ULONG) CRibbonCommandHandler::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) CRibbonCommandHandler::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}

		return cRef;
	}

	STDMETHODIMP CRibbonCommandHandler::QueryInterface(REFIID iid, void** ppv)
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
	STDMETHODIMP CRibbonCommandHandler::UpdateProperty(UINT nCmdID, __in REFPROPERTYKEY key, __in_opt const PROPVARIANT* ppropvarCurrentValue, 
												 __out PROPVARIANT* ppropvarNewValue)
	{
		UNREFERENCED_PARAMETER(nCmdID);
		UNREFERENCED_PARAMETER(key);
		UNREFERENCED_PARAMETER(ppropvarCurrentValue);
		UNREFERENCED_PARAMETER(ppropvarNewValue);

		return E_NOTIMPL;
	}

	// Called by the Ribbon framework when a command is executed by the user, eg. when a button is pressed.
	STDMETHODIMP CRibbonCommandHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
										  __in_opt IUISimplePropertySet* pCommandExecutionProperties)
	{
		// Note: _USE_RIBBON_ must be defined to use this function
		return m_pFrame->OnRibbonExecute(nCmdID, verb, key, ppropvarValue, pCommandExecutionProperties);
	}

}