/////////////////////////////////
// Ribbon.h

#ifndef _RIBBON_H_
#define _RIBBON_H_

#include <wincore.h>
#include <UIRibbon.h>

namespace Win32xx
{

	// Defines the methods for gathering Command information and handling Command events from the Ribbon framework.
	class CRibbonCommandHandler : public IUICommandHandler
	{
	public:

		// Static method to create an instance of the object.
		__checkReturn static HRESULT CreateInstance(__deref_out IUICommandHandler **ppCommandHandler);

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
		CRibbonCommandHandler() : m_cRef(1) {}
		CWnd* m_pFrame;
		LONG m_cRef;                        // Reference count.
	};


	// Defines the callback entry-point methods for the Ribbon framework.
	class CRibbonApplication : public IUIApplication
	{
	public:

		// Static method to create an instance of the object.
		static HRESULT CreateInstance(__deref_out IUIApplication **ppApplication);

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

	private:
		CRibbonApplication() : m_cRef(1) , m_pCommandHandler(NULL) {}

		~CRibbonApplication() 
		{
			if (m_pCommandHandler)
			{
				m_pCommandHandler->Release();
				m_pCommandHandler = NULL;
			}
		}

		LONG m_cRef;                            // Reference count.
		CWnd* m_pFrame;
		CRibbonCommandHandler* m_pCommandHandler;
	};

}

#endif  // RIBBON_H

