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

#include "wincore.h"
#include <UIRibbon.h>

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



#endif  // RIBBON_H

