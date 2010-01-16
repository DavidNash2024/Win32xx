/////////////////////////////////
// Application.h

#ifndef APPLICATION_H
#define APPLICATION_H

#include <wincore.h>
#include <UIRibbon.h>

// Defines the callback entry-point methods for the Ribbon framework.
class CRibbonManager : public IUIApplication
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
    CRibbonManager() : m_cRef(1) , m_pCommandHandler(NULL) {}

    ~CRibbonManager() 
    {
        if (m_pCommandHandler)
        {
            m_pCommandHandler->Release();
            m_pCommandHandler = NULL;
        }
    }

    LONG m_cRef;                            // Reference count.
	CWnd* m_pFrame;
    IUICommandHandler * m_pCommandHandler;  // Generic Command Handler

};


#endif  // APPLICATION_H

