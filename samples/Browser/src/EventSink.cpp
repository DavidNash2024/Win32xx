#include "stdafx.h"
#include "EventSink.h"
#include "MainFrm.h"


CEventSink::CEventSink() : m_cRefs(1), m_pSink(NULL)
{
}

STDMETHODIMP_(ULONG) CEventSink::AddRef()
{
    return ++m_cRefs;
}

STDMETHODIMP CEventSink::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;

    if (IsEqualIID(riid, IID_IDispatch))
        *ppvObject = (IDispatch *)this;
    else if (IsEqualIID(riid, IID_IUnknown))
        *ppvObject = this;
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

STDMETHODIMP CEventSink::GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, unsigned int cNames, LCID lcid, DISPID* rgdispid)
{
    UNREFERENCED_PARAMETER((IID)riid);          // IID cast required for the MinGW compiler
    UNREFERENCED_PARAMETER(rgszNames);
    UNREFERENCED_PARAMETER(cNames);
    UNREFERENCED_PARAMETER(lcid);
    *rgdispid = DISPID_UNKNOWN;
    return DISP_E_UNKNOWNNAME;
}

STDMETHODIMP CEventSink::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo)
{
    UNREFERENCED_PARAMETER(itinfo);
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(pptinfo);
    return E_NOTIMPL;
}

STDMETHODIMP CEventSink::GetTypeInfoCount(unsigned int* pctinfo)
{
    UNREFERENCED_PARAMETER(pctinfo);
    return E_NOTIMPL;
}

STDMETHODIMP CEventSink::Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pvarResult, EXCEPINFO* pexecinfo, unsigned int* puArgErr)
{
    UNREFERENCED_PARAMETER((IID)riid);          // IID cast required for the MinGW compiler
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(wFlags);
    UNREFERENCED_PARAMETER(pvarResult);
    UNREFERENCED_PARAMETER(pexecinfo);
    UNREFERENCED_PARAMETER(puArgErr);

    if (!m_pSink)
        return S_OK;

    switch (dispid)
    {
        // Just some of the possible events ...
        case DISPID_BEFORENAVIGATE2:
            m_pSink->OnBeforeNavigate2(pDispParams);
            break;

        case DISPID_COMMANDSTATECHANGE:
            m_pSink->OnCommandStateChange(pDispParams);
            break;

        case DISPID_DOCUMENTCOMPLETE:
            m_pSink->OnDocumentComplete(pDispParams);
            break;

        case DISPID_DOWNLOADBEGIN:
            m_pSink->OnDownloadBegin(pDispParams);
            break;

        case DISPID_DOWNLOADCOMPLETE:
            m_pSink->OnDownloadComplete(pDispParams);
            break;

        case DISPID_NAVIGATECOMPLETE2:
            m_pSink->OnNavigateComplete2(pDispParams);
            break;

        case DISPID_NEWWINDOW2:
            m_pSink->OnNewWindow2(pDispParams);
            break;

        case DISPID_PROGRESSCHANGE:
            m_pSink->OnProgressChange(pDispParams);
            break;

        case DISPID_PROPERTYCHANGE:
            m_pSink->OnPropertyChange(pDispParams);
            break;

        case DISPID_STATUSTEXTCHANGE:
            m_pSink->OnStatusTextChange(pDispParams);
            break;

        case DISPID_TITLECHANGE:
            m_pSink->OnTitleChange(pDispParams);
            break;
    }

    return S_OK;
}

STDMETHODIMP_(ULONG) CEventSink::Release()
{
    return --m_cRefs;
}

