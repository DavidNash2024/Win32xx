// Win32++  Version 6.8 alpha
// Released: ?th February, 2010 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// ribbon.h
//  Declaration of the following classes:
//  CRibbonCommandHandler and CRibbon
//

#ifndef _RIBBON_H_
#define _RIBBON_H_


#include <UIRibbon.h>		// Contained within the Windows 7 SDK	
#include "wincore.h"

namespace Win32xx
{

	// Defines the methods for gathering Command information and handling Command events from the Ribbon framework.
	class CRibbonCommandHandler : public IUICommandHandler
	{
	public:
		CRibbonCommandHandler() : m_cRef(1), m_pFrame(NULL) {}
		~CRibbonCommandHandler(); 

		// IUnknown methods.
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();
		STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

		// IUICommandHandler methods
		STDMETHOD(UpdateProperty)(UINT nCmdID,
			__in REFPROPERTYKEY key,
			__in_opt const PROPVARIANT* ppropvarCurrentValue,
			__out PROPVARIANT* ppropvarNewValue);

		STDMETHOD(Execute)(UINT nCmdID,
			UI_EXECUTIONVERB verb, 
			__in_opt const PROPERTYKEY* key,
			__in_opt const PROPVARIANT* ppropvarValue,
			__in_opt IUISimplePropertySet* pCommandExecutionProperties);

		CWnd* GetFrame() const { return m_pFrame; }
		void SetFrame(CWnd* pFrame) { m_pFrame = pFrame; }

	private:
		CWnd* m_pFrame;
		LONG m_cRef;                        // Reference count.
	};

	// Defines the callback entry-point methods for the Ribbon framework.
	class CRibbon : public IUIApplication
	{
	public:
		CRibbon() : m_cRef(1), m_pCommandHandler(NULL), m_pRibbonFramework(NULL), m_pFrame(NULL) {}
		~CRibbon(); 

		// IUnknown methods.
		STDMETHOD_(ULONG, AddRef());
		STDMETHOD_(ULONG, Release());
		STDMETHOD(QueryInterface(REFIID iid, void** ppv));

		// IUIApplication methods
		STDMETHOD(OnCreateUICommand)(UINT nCmdID, __in UI_COMMANDTYPE typeID, 
			__deref_out IUICommandHandler** ppCommandHandler);

		STDMETHOD(OnViewChanged)(UINT viewId, __in UI_VIEWTYPE typeId, __in IUnknown* pView,
			UI_VIEWVERB verb, INT uReasonCode);

		STDMETHOD(OnDestroyUICommand)(UINT32 commandId, __in UI_COMMANDTYPE typeID,
			__in_opt IUICommandHandler* commandHandler);

		CWnd* GetFrame() const { return m_pFrame; }
		void SetFrame(CWnd* pFrame) { m_pFrame = pFrame; }

		bool CreateRibbon(CWnd* pWnd);
		void DestroyRibbon();

	private:
		LONG m_cRef;                            // Reference count.
		CWnd* m_pFrame;
		CRibbonCommandHandler* m_pCommandHandler;
		IUIFramework* m_pRibbonFramework;
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
	//////////////////////////////////////////////
	// Definitions for the CRibbon class
	//

	inline CRibbon::~CRibbon() 
		{
			// Reference count must be 1 or we have a leak!
			assert(m_cRef == 1);
			
			if (m_pCommandHandler)
			{
				delete m_pCommandHandler;
			}
		}

	// IUnknown method implementations.
	inline STDMETHODIMP_(ULONG) CRibbon::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	inline STDMETHODIMP_(ULONG) CRibbon::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		return cRef;
	}

	inline STDMETHODIMP CRibbon::QueryInterface(REFIID iid, void** ppv)
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

	// Called by the Ribbon framework for each command specified in markup, to allow
	inline STDMETHODIMP CRibbon::OnCreateUICommand(UINT nCmdID, __in UI_COMMANDTYPE typeID, 
												 __deref_out IUICommandHandler** ppCommandHandler)
	{
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(nCmdID);

		if (NULL == m_pCommandHandler)
		{
			m_pCommandHandler = new CRibbonCommandHandler();		
			m_pCommandHandler->SetFrame(m_pFrame);
		}

		return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
	}

	// Called when the state of the Ribbon changes, for example, created, destroyed, or resized.
	inline STDMETHODIMP CRibbon::OnViewChanged(UINT viewId, __in UI_VIEWTYPE typeId, __in IUnknown* pView, 
											 UI_VIEWVERB verb, INT uReasonCode)
	{
		if (!m_pFrame)
		{
			TRACE(_T("CRibbonCommandHandler m_pFrame not set\n"));
			return E_NOTIMPL;
		}

		return GetFrame()->RibbonOnViewChanged(viewId, typeId, pView, verb, uReasonCode);
	}

	// Called by the Ribbon framework for each command at the time of ribbon destruction.
	inline STDMETHODIMP CRibbon::OnDestroyUICommand(UINT32 nCmdID, __in UI_COMMANDTYPE typeID,
												  __in_opt IUICommandHandler* commandHandler)
	{
		UNREFERENCED_PARAMETER(commandHandler);
		UNREFERENCED_PARAMETER(typeID);
		UNREFERENCED_PARAMETER(nCmdID);

		return E_NOTIMPL;
	}

	inline bool CRibbon::CreateRibbon(CWnd* pWnd)
	{
		
		::CoInitialize(NULL);

		// Instantiate the Ribbon framework object.
		::CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pRibbonFramework));

		SetFrame(pWnd);

		// Connect the host application to the Ribbon framework.
		HRESULT hr = m_pRibbonFramework->Initialize(pWnd->GetHwnd(), this);
		if (FAILED(hr))
		{
			return false;
		}

		// Load the binary markup. APPLICATION_RIBBON is the default name generated by uicc.
		hr = m_pRibbonFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	inline void CRibbon::DestroyRibbon()
	{
		if (m_pRibbonFramework)
		{
			m_pRibbonFramework->Destroy();
			m_pRibbonFramework->Release();
			m_pRibbonFramework = NULL;
		}
	}

	
	/////////////////////////////////////////////////
	// Definitions for the CRibbonCommandHandler class
	//

	inline CRibbonCommandHandler::~CRibbonCommandHandler()
	{
		// Reference count must be 1 or we have a leak!
		assert(m_cRef == 1);
	}

	// IUnknown method implementations.
	inline STDMETHODIMP_(ULONG) CRibbonCommandHandler::AddRef()
	{
		if (!m_pFrame)
		{
			TRACE(_T("CRibbonCommandHandler m_pFrame not set\n"));
			return 0L;
		}
		
		return InterlockedIncrement(&m_cRef);
	}

	inline STDMETHODIMP_(ULONG) CRibbonCommandHandler::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		return cRef;
	}

	inline STDMETHODIMP CRibbonCommandHandler::QueryInterface(REFIID iid, void** ppv)
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

	// Called by the Ribbon framework when a command property (PKEY) needs to be updated.
	inline STDMETHODIMP CRibbonCommandHandler::UpdateProperty(UINT nCmdID, __in REFPROPERTYKEY key, __in_opt const PROPVARIANT* ppropvarCurrentValue, 
												 __out PROPVARIANT* ppropvarNewValue)
	{
		UNREFERENCED_PARAMETER(nCmdID);
		UNREFERENCED_PARAMETER(key);
		UNREFERENCED_PARAMETER(ppropvarCurrentValue);
		UNREFERENCED_PARAMETER(ppropvarNewValue);

		return E_NOTIMPL;
	}

	// Called by the Ribbon framework when a command is executed by the user, eg. when a button is pressed.
	inline STDMETHODIMP CRibbonCommandHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
										  __in_opt IUISimplePropertySet* pCommandExecutionProperties)
	{
		if (!m_pFrame)
		{
			TRACE(_T("CRibbonCommandHandler m_pFrame not set\n"));
			return E_NOTIMPL;
		}
		
		return m_pFrame->RibbonExecute(nCmdID, verb, key, ppropvarValue, pCommandExecutionProperties);
	}

}

#endif  // RIBBON_H

