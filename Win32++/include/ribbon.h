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
//  CRibbon and CRibbonFrame
//

#ifndef _RIBBON_H_
#define _RIBBON_H_


// Notes: 1) The Windows 7 SDK must be installed and its directories added to the IDE
//        2) The ribbon only works on OS Windows 7 and above

#include <UIRibbon.h>		// Contained within the Windows 7 SDK	


namespace Win32xx
{
	// Defines the callback entry-point methods for the Ribbon framework.
	class CRibbon : public IUICommandHandler, public IUIApplication
	{
	public:
		CRibbon() : m_cRef(1), m_pRibbonFramework(NULL) {}
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

		// IUICommandHandle methods
		STDMETHODIMP Execute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
										  __in_opt IUISimplePropertySet* pCommandExecutionProperties);

		STDMETHODIMP UpdateProperty(UINT nCmdID, __in REFPROPERTYKEY key, __in_opt const PROPVARIANT* ppropvarCurrentValue, 
												 __out PROPVARIANT* ppropvarNewValue);	

		bool virtual CreateRibbon(CWnd* pWnd);
		void virtual DestroyRibbon();

	private:
		IUIFramework* m_pRibbonFramework;
		LONG m_cRef;                            // Reference count.
	};


	class CRibbonFrame : public CFrame, public CRibbon
	{
	public:
		CRibbonFrame() : m_uRibbonHeight(0) {}
		virtual ~CRibbonFrame() {}
		virtual CRect GetViewRect() const;
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual STDMETHODIMP OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode);
		
		UINT GetRibbonHeight() const { return m_uRibbonHeight; }
		void SetRibbonHeight(UINT uRibbonHeight) { m_uRibbonHeight = uRibbonHeight; }

	private:
		UINT m_uRibbonHeight;
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

	inline STDMETHODIMP CRibbon::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
										  __in_opt IUISimplePropertySet* pCommandExecutionProperties)
	{
		return E_NOTIMPL;
	}

	inline STDMETHODIMP CRibbon::QueryInterface(REFIID iid, void** ppv)
	{
		if (iid == __uuidof(IUnknown))
		{
			*ppv = static_cast<IUnknown*>(static_cast<IUIApplication*>(this));
		}
		else if (iid == __uuidof(IUICommandHandler))
		{
			*ppv = static_cast<IUICommandHandler*>(this);
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

		return QueryInterface(IID_PPV_ARGS(ppCommandHandler));
	}

	// Called when the state of the Ribbon changes, for example, created, destroyed, or resized.
	inline STDMETHODIMP CRibbon::OnViewChanged(UINT viewId, __in UI_VIEWTYPE typeId, __in IUnknown* pView, 
											 UI_VIEWVERB verb, INT uReasonCode)
	{
		return E_NOTIMPL;
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

	// Called by the Ribbon framework when a command property (PKEY) needs to be updated.
	inline STDMETHODIMP CRibbon::UpdateProperty(UINT nCmdID, __in REFPROPERTYKEY key, __in_opt const PROPVARIANT* ppropvarCurrentValue, 
												 __out PROPVARIANT* ppropvarNewValue)
	{
		UNREFERENCED_PARAMETER(nCmdID);
		UNREFERENCED_PARAMETER(key);
		UNREFERENCED_PARAMETER(ppropvarCurrentValue);
		UNREFERENCED_PARAMETER(ppropvarNewValue);

		return E_NOTIMPL;
	}

	inline bool CRibbon::CreateRibbon(CWnd* pWnd)
	{	
		::CoInitialize(NULL);

		// Instantiate the Ribbon framework object.
		::CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pRibbonFramework));

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
	
	
	//////////////////////////////////////////////
	// Definitions for the CRibbonFrame class
	//

	inline CRect CRibbonFrame::GetViewRect() const
	{
		// Get the frame's client area
		CRect rcFrame = GetClientRect();

		// Get the statusbar's window area
		CRect rcStatus;
		if (GetStatusbar().IsWindowVisible() || !IsWindowVisible())
			rcStatus = GetStatusbar().GetWindowRect();

		// Get the top rebar or toolbar's window area
		CRect rcTop;
		if (IsRebarSupported() && m_bUseRebar)
			rcTop = GetRebar().GetWindowRect();
		else
			if (GetToolbar().IsWindowVisible())
				rcTop = GetToolbar().GetWindowRect();

		// Return client size less the rebar and status windows
		int top = rcFrame.top + rcTop.Height() + m_uRibbonHeight;
		int left = rcFrame.left;
		int right = rcFrame.right;
		int bottom = rcFrame.Height() - (rcStatus.Height());
		if ((bottom <= top) ||( right <= left))
			top = left = right = bottom = 0;

		CRect rcView(left, top, right, bottom);
		return rcView;
	}

	inline void CRibbonFrame::OnCreate()
	{
		// OnCreate is called automatically during window creation when a
		// WM_CREATE message received.

		// Tasks such as setting the icon, creating child windows, or anything
		// associated with creating windows are normally performed here.

		if (GetWinVersion() >= 2601)
		{		
			m_bUseRebar = FALSE;			// Don't use rebars
			m_bUseToolbar = FALSE;			// Don't use a toolbar
			
			CFrame::OnCreate();

			if (CreateRibbon(this))
				TRACE(_T("Ribbon Created Succesfully\n"));
			else
				throw CWinException(_T("Failed to create ribbon"));
		}
		else
		{
			CFrame::OnCreate();
		}

	}

	inline void CRibbonFrame::OnDestroy()
	{
		DestroyRibbon();
		CFrame::OnDestroy();
	}

	inline STDMETHODIMP CRibbonFrame::OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode)
	{
		UNREFERENCED_PARAMETER(viewId);
		UNREFERENCED_PARAMETER(uReasonCode);

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
						SetRibbonHeight(uRibbonHeight);
						pRibbon->Release();

						RecalcLayout();
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

} 

#endif  // RIBBON_H

