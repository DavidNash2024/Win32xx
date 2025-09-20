/////////////////////////////
// DialogEventHandler.cpp
//

// This sample is based on the CommonFileDialog and the CommFileDialogModes
// samples that ships with the Windows 7 SDK. The original sample can be
// downloaded from :
// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/CommonFileDialogModes
// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog

#include "stdafx.h"           // Adds precompiled headers
#define STRICT_TYPED_ITEMIDS  // Improves the type safety of IDLists
#include <knownfolders.h>     // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>      // for PROPVAR-related functions
#include <propkey.h>          // for the Property key APIs/datatypes
#include <wrl.h>              // for Microsoft::WRL::ComPtr

#include "DialogEventHandler.h"
#include "resource.h"

using namespace Microsoft::WRL;

inline void ReportSelectedItems(IShellItemArray* psia)
{
    DWORD cItems;
    if (SUCCEEDED(psia->GetCount(&cItems)))
    {
        std::vector<CString> itemNames;
        for (DWORD i = 0; i < cItems; i++)
        {
            ComPtr<IShellItem> psi;
            if (SUCCEEDED(psia->GetItemAt(i, &psi)))
            {
                SFGAOF attribute = 0;
                if (SUCCEEDED(psi->GetAttributes(SFGAO_FOLDER, &attribute)))
                {
                    if (!(attribute & SFGAO_FOLDER)) // Skip any folders selected
                    {
                        PWSTR pszName;
                        if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszName)))
                        {
                            itemNames.push_back(pszName);
                            CoTaskMemFree(pszName);
                        }
                    }
                }
            }
        }

        size_t items = itemNames.size();
        for (size_t i = 0; i < items; i++)
        {
            const TASKDIALOG_COMMON_BUTTON_FLAGS buttonFlags = (i+1 == items) ?
                TDCBF_OK_BUTTON : TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;

            CString message;
            message << "Item " << i+1 << " of " << items;
            CString title;
            title << items << (items > 1 ? " items" : " item") << " selected";
            TaskDialog(nullptr, 0, title, message, itemNames[i], buttonFlags, nullptr, nullptr);
        }
    }
}

// This method gets called when the file-type is changed (combo-box selection changes).
// For sample sake, let's react to this event by changing the properties show.
IFACEMETHODIMP CDialogEventHandler::OnTypeChange(IFileDialog *pfd)
{
    ComPtr<IFileSaveDialog> pfsd;
    HRESULT hr = pfd->QueryInterface(IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        UINT uIndex;
        hr = pfsd->GetFileTypeIndex(&uIndex);   // index of current file-type
        if (SUCCEEDED(hr))
        {
            ComPtr<IPropertyDescriptionList> pdl;
            switch (uIndex)
            {
            case INDEX_WORDDOC:
                // When .doc is selected, let's ask for some arbitrary property, say Title.
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Title", IID_PPV_ARGS(&pdl));
                if (SUCCEEDED(hr))
                {
                    // FALSE as second param == do not show default properties.
                    hr = pfsd->SetCollectedProperties(pdl.Get(), FALSE);
                }
                break;

            case INDEX_WEBPAGE:
                // When .html is selected, let's ask for some other arbitrary property, say Keywords.
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Keywords", IID_PPV_ARGS(&pdl));
                if (SUCCEEDED(hr))
                {
                    // FALSE as second param == do not show default properties.
                    hr = pfsd->SetCollectedProperties(pdl.Get(), FALSE);
                }
                break;

            case INDEX_TEXTDOC:
                // When .txt is selected, let's ask for some other arbitrary property, say Author.
                hr = PSGetPropertyDescriptionListFromString(L"prop:System.Author", IID_PPV_ARGS(&pdl));
                if (SUCCEEDED(hr))
                {
                    // TRUE as second param == show default properties as well, but show Author property first in list.
                    hr = pfsd->SetCollectedProperties(pdl.Get(), TRUE);
                }
                break;
            }
        }
    }
    return hr;
}

// This method gets called when an dialog control item selection happens (radio-button selection. etc).
// For sample sake, let's react to this event by changing the dialog title.
// This method is used by AddCustomControls.
IFACEMETHODIMP CDialogEventHandler::OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem)
{
    ComPtr<IFileDialog> pfd;
    HRESULT hr = pfdc->QueryInterface(IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        if (dwIDCtl == CONTROL_RADIOBUTTONLIST)
        {
            switch (dwIDItem)
            {
            case CONTROL_RADIOBUTTON1:
                hr = pfd->SetTitle(L"Longhorn Dialog");
                break;

            case CONTROL_RADIOBUTTON2:
                hr = pfd->SetTitle(L"Vista Dialog");
                break;
            }
        }
    }
    return hr;
}


IFACEMETHODIMP CDialogEventHandler::OnFileOk(IFileDialog* pfd)
{
    // Only for FileOpenDialog. Ignored for FileSaveDialog.
    ComPtr<IFileOpenDialog> pfod;
    HRESULT hr = pfd->QueryInterface(IID_PPV_ARGS(&pfod));

    if (SUCCEEDED(hr))
    {
        ComPtr<IShellItemArray> psia;
        hr = pfod->GetSelectedItems(&psia);
        if (SUCCEEDED(hr))
        {
            ReportSelectedItems(psia.Get());
        }
    }

    return S_OK;  // S_OK to allows the dialog to dismiss. return S_FALSE keep it open.
}
