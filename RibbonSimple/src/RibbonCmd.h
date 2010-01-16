/////////////////////////////////
// CommandHandler.h

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H


// Defines the methods for gathering Command information and handling Command events from the Ribbon framework.
class CRibbonCommands : public IUICommandHandler
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

	CWnd* GetFrame() const {return m_pFrame;}
	void SetFrame(CWnd* pFrame) {m_pFrame = pFrame;}

private:
    CRibbonCommands() : m_cRef(1) {}
	CWnd* m_pFrame;
    LONG m_cRef;                        // Reference count.
};

#endif  // COMMANDHANDLER_H