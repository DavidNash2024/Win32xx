/////////////////////////////////
// CommandHandler.h

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H


class CCommandHandler
    : public IUICommandHandler // Command handlers must implement IUICommandHandler.
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

private:
    CCommandHandler()
        : m_cRef(1) 
    {
    }

    LONG m_cRef;                        // Reference count.
};

#endif  // COMMANDHANDLER_H