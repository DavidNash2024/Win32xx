///////////////////////////////
// Application.cpp

#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include "Application.h"
#include "CommandHandler.h"

// Static method to create an instance of the object.
__checkReturn HRESULT CRibbonManager::CreateInstance(__deref_out IUIApplication **ppApplication)
{
    if (!ppApplication)
    {
        return E_POINTER;
    }

    *ppApplication = NULL;

    HRESULT hr = S_OK;
   
    CRibbonManager* pApplication = new CRibbonManager();

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
STDMETHODIMP_(ULONG) CRibbonManager::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CRibbonManager::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CRibbonManager::QueryInterface(REFIID iid, void** ppv)
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

STDMETHODIMP CRibbonManager::OnCreateUICommand(UINT nCmdID, __in UI_COMMANDTYPE typeID, 
											 __deref_out IUICommandHandler** ppCommandHandler)
{
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    if (NULL == m_pCommandHandler)
    {
        HRESULT hr = CRibbonCommands::CreateInstance(&m_pCommandHandler);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
}

//
//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
//
//  PURPOSE: Called when the state of a View (Ribbon is a view) changes, for example, created, destroyed, or resized.
//
//
STDMETHODIMP CRibbonManager::OnViewChanged(UINT viewId, __in UI_VIEWTYPE typeId, __in IUnknown* pView, 
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
STDMETHODIMP CRibbonManager::OnDestroyUICommand(UINT32 nCmdID, __in UI_COMMANDTYPE typeID,
											  __in_opt IUICommandHandler* commandHandler)
{
    UNREFERENCED_PARAMETER(commandHandler);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    return E_NOTIMPL;
}

