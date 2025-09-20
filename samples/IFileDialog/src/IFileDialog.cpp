/////////////////////////
// IFileDialog.cpp
//

// This sample is based on the CommonFileDialog and the CommFileDialogModes
// samples that ships with the Windows 7 SDK. The original sample can be
// downloaded from :
// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/CommonFileDialogModes
// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog


#include "stdafx.h"           // Adds precompied headers
#define STRICT_TYPED_ITEMIDS  // Improves the type safety of IDLists
#include <knownfolders.h>     // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>      // for PROPVAR-related functions
#include <propkey.h>          // for the Property key APIs/datatypes
#include <wrl.h>              // for Microsoft::WRL::ComPtr

#include "DialogEventHandler.h"
#include "resource.h"

using Microsoft::WRL::ComPtr;

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"Word Document (*.doc)",       L"*.doc"},
    {L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
    {L"Text Document (*.txt)",       L"*.txt"},
    {L"All Documents (*.*)",         L"*.*"}
};


//////////////////////
// Utility Functions

// Helper function to add a key and value to the specified property store.
void AddPropertyToStore(IPropertyStore* pps, REFPROPERTYKEY key, LPCWSTR value)
{
    PROPVARIANT variant = {};
    if (SUCCEEDED(InitPropVariantFromString(value, &variant)))
    {
        if (SUCCEEDED(pps->SetValue(key, variant)))
            pps->Commit();

        PropVariantClear(&variant);
    }
}

// Helper function to write property/value into a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
void AppendPropertyToCustomFile(PCWSTR pszFileName, PCWSTR pszPropertyName, PCWSTR pszValue)
{
    try
    {
        CString propertyLine = "[PROPERTY]";
        propertyLine << pszPropertyName << "=" << pszValue << "[ENDPROPERTY]\r\n";

        CFile file(pszFileName, OPEN_ALWAYS);
        file.SeekToEnd();
        CStringA dataIn(WtoA(propertyLine).c_str());
        file.Write(dataIn.c_str(), dataIn.GetLength());
    }

    catch (...)
    {
        TaskDialog(nullptr, nullptr, L"Error Appending to File: ", pszFileName, nullptr,
            TDCBF_OK_BUTTON, TD_ERROR_ICON, nullptr);
    }
}


// Helper function to write dummy content to a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
void WriteDataToCustomFile(PCWSTR pszFileName)
{
    try
    {
        CFile file(pszFileName, OPEN_ALWAYS);
        CStringA dummyContent = "[MYAPPDATA]\r\n";
        dummyContent += "This is an example of how to use the IFileSaveDialog interface.\r\n";
        dummyContent += "[ENDMYAPPDATA]\r\n";

        file.Write(dummyContent.c_str(), dummyContent.GetLength());
    }

    catch (...)
    {
        TaskDialog(nullptr, nullptr, L"Error Writing to File: ", pszFileName, nullptr,
            TDCBF_OK_BUTTON, TD_ERROR_ICON, nullptr);
    }
}


//////////////////////////////
// Common File Dialog Snippets


// The ChooseFile code snippet demonstrates how to choose a file with the
// common filedialog interface.
HRESULT ChooseFile()
{
    // CoCreate the File Open Dialog object.
    ComPtr<IFileDialog> pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        // Set the options on the dialog. Before setting, always get the options first
        // in order not to override existing options.
        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            // In this case, get shell items only for file system items.
            hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                // Set the file types to display only. Notice that, this is a 1-based array.
                hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                if (SUCCEEDED(hr))
                {
                    // Set the selected file type index to Word Docs for this example.
                    hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
                    if (SUCCEEDED(hr))
                    {
                        // Set the default extension to be ".doc" file.
                        hr = pfd->SetDefaultExtension(L"doc");
                        if (SUCCEEDED(hr))
                        {
                            // Show the dialog.
                            hr = pfd->Show(nullptr);
                            if (SUCCEEDED(hr))
                            {
                                // Obtain the result, once the user clicks the 'Open' button.
                                // The result is an IShellItem object.
                                ComPtr<IShellItem> psiResult;
                                hr = pfd->GetResult(&psiResult);
                                if (SUCCEEDED(hr))
                                {
                                    // We are just going to print out the name of the file for sample sake.
                                    PWSTR pszFilePath = nullptr;
                                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                    if (SUCCEEDED(hr))
                                    {
                                        TaskDialog(nullptr, nullptr, L"Chosen File: ", pszFilePath, nullptr,
                                                   TDCBF_OK_BUTTON, TD_INFORMATION_ICON, nullptr);

                                        CoTaskMemFree(pszFilePath);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return hr;
}

// This code snippet demonstrates how to choose a folder with the common file
// dialog interface.
HRESULT ChooseFolder()
{
    ComPtr<IFileDialog> pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        DWORD dwOptions;
        hr = pfd->GetOptions(&dwOptions);
        if (SUCCEEDED(hr))
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

        hr = pfd->Show(nullptr);
        if (SUCCEEDED(hr))
        {
            ComPtr<IShellItem> psi;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath = 0;
                hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);
                if (SUCCEEDED(hr))
                {
                    TaskDialog(nullptr, nullptr, L"Choosen Folder:", pszFilePath, 0,
                        TDCBF_OK_BUTTON, TD_INFORMATION_ICON, nullptr);

                    CoTaskMemFree(pszFilePath);
                }
                else
                    TaskDialog(nullptr, nullptr, L"Choosen Folder:", L"GetDisplayName failed", 0,
                        TDCBF_OK_BUTTON, TD_ERROR_ICON, nullptr);
            }
        }
    }

    return hr;
}

// This sample demonstrates how to use the file dialog in a modal way such that
// users can easily pick multiple files. It does this by overriding the normal "Open" button
// with an "Add" button that passes the selection back to this app.
HRESULT ChooseMultipleFiles()
{
    ComPtr<IFileOpenDialog> pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        CDialogEventHandler foacb;
        DWORD dwCookie;
        if (SUCCEEDED(hr = pfd->Advise(&foacb, &dwCookie)))
        {
            DWORD dwOptions;
            if (SUCCEEDED(hr = pfd->GetOptions(&dwOptions)))
            {
                if (SUCCEEDED(hr = pfd->SetOptions(dwOptions | FOS_ALLOWMULTISELECT | FOS_ALLNONSTORAGEITEMS)))
                {
                    // Set the file types to display only. Notice that, this is a 1-based array.
                    hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                    if (SUCCEEDED(hr))
                    {
                        pfd->SetTitle(L"Open Multiple Files");

                        // We do not process the results of the dialog since
                        // the selected items are passed back via OnFileOk()
                        hr = pfd->Show(nullptr); // hr intentionally ignored
                    }
                }
            }
        }
    }

    return hr;
}

// This code snippet demonstrates how to add custom controls in the Common File Dialog.
HRESULT AddCustomControls()
{
    // CoCreate the File Open Dialog object.
    ComPtr<IFileDialog> pfd;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        // Create an event handling object, and hook it up to the dialog.
        DWORD               dwCookie    = 0;
        CDialogEventHandler dfe;

        // Hook up the event handler.
        hr = pfd->Advise(&dfe, &dwCookie);
        if (SUCCEEDED(hr))
        {
            // Set up a Customization.
            ComPtr<IFileDialogCustomize> pfdc;
            hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
            if (SUCCEEDED(hr))
            {
                // Create a Visual Group.
                hr = pfdc->StartVisualGroup(CONTROL_GROUP, L"Sample Group");
                if (SUCCEEDED(hr))
                {
                    // Add a radio-button list.
                    hr = pfdc->AddRadioButtonList(CONTROL_RADIOBUTTONLIST);
                    if (SUCCEEDED(hr))
                    {
                        // Set the state of the added radio-button list.
                        hr = pfdc->SetControlState(CONTROL_RADIOBUTTONLIST, CDCS_VISIBLE | CDCS_ENABLED);
                        if (SUCCEEDED(hr))
                        {
                            // Add individual buttons to the radio-button list.
                            hr = pfdc->AddControlItem(CONTROL_RADIOBUTTONLIST,
                                                      CONTROL_RADIOBUTTON1,
                                                      L"Change Title to Longhorn");
                            if (SUCCEEDED(hr))
                            {
                                hr = pfdc->AddControlItem(CONTROL_RADIOBUTTONLIST,
                                                          CONTROL_RADIOBUTTON2,
                                                          L"Change Title to Vista");
                                if (SUCCEEDED(hr))
                                {
                                    // Set the default selection to option 1.
                                    hr = pfdc->SetSelectedControlItem(CONTROL_RADIOBUTTONLIST,
                                                                      CONTROL_RADIOBUTTON1);
                                }
                            }
                        }
                    }
                    // End the visual group.
                    pfdc->EndVisualGroup();
                }
            }

            if (FAILED(hr))
            {
                // Unadvise here in case we encounter failures before we get a chance to show the dialog.
                pfd->Unadvise(dwCookie);
            }
        }

        if (SUCCEEDED(hr))
        {
            // Now show the dialog.
            hr = pfd->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                //
                // You can add your own code here to handle the results.
                //
            }
            // Unhook the event handler.
            pfd->Unadvise(dwCookie);
        }
    }
    return hr;
}

// This code snippet demonstrates how to add default metadata in the Common File Dialog.
// Look at CDialogEventHandler::OnTypeChange to see to change the order/list of properties
// displayed in the Common File Dialog.
HRESULT SetDefaultValuesForProperties()
{
    // CoCreate the File Save Dialog object.
    ComPtr<IFileSaveDialog> pfsd;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        // Create an event handling object, and hook it up to the dialog.
        CDialogEventHandler fde;
        DWORD               dwCookie    = 0;

        // Hook up the event handler.
        hr = pfsd->Advise(&fde, &dwCookie);
        if (SUCCEEDED(hr))
        {
            // Set the file types to display.
            hr = pfsd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
            if (SUCCEEDED(hr))
            {
                hr = pfsd->SetFileTypeIndex(INDEX_WORDDOC);
                if (SUCCEEDED(hr))
                {
                    hr = pfsd->SetDefaultExtension(L"doc");
                    if (SUCCEEDED(hr))
                    {
                        // The InMemory Property Store is a Property Store that is
                        // kept in the memory instead of persisted in a file stream.
                        ComPtr<IPropertyStore> pps;
                        hr = PSCreateMemoryPropertyStore(IID_PPV_ARGS(&pps));
                        if (SUCCEEDED(hr))
                        {
                            PROPVARIANT propvarValue = {};
                            hr = InitPropVariantFromString(L"SampleKeywordsValue", &propvarValue);
                            if (SUCCEEDED(hr))
                            {
                                // Set the value to the property store of the item.
                                hr = pps->SetValue(PKEY_Keywords, propvarValue);
                                if (SUCCEEDED(hr))
                                {
                                    // Commit does the actual writing back to the in memory store.
                                    hr = pps->Commit();
                                    if (SUCCEEDED(hr))
                                    {
                                        // Hand these properties to the File Dialog.
                                        hr = pfsd->SetCollectedProperties(nullptr, TRUE);
                                        if (SUCCEEDED(hr))
                                        {
                                            hr = pfsd->SetProperties(pps.Get());
                                        }
                                    }
                                }
                                PropVariantClear(&propvarValue);
                            }
                        }
                    }
                }
            }

            if (FAILED(hr))
            {
                // Unadvise here in case we encounter failures before we get a chance to show the dialog.
                pfsd->Unadvise(dwCookie);
            }
        }

        if (SUCCEEDED(hr))
        {
            // Now show the dialog.
            hr = pfsd->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                //
                // You can add your own code here to handle the results.
                //
            }
            // Unhook the event handler.
            pfsd->Unadvise(dwCookie);
        }
    }
    return hr;
}

// The following code snippet demonstrates two things:
// 1.  How to write properties using property handlers.
// 2.  Replicating properties in the "Save As" scenario where the user choses to save
//     an existing file with a different name.  We need to make sure we replicate not
//     just the data, but also the properties of the original file.
HRESULT WritePropertiesUsingHandlers()
{
    // CoCreate the File Save Dialog object.
    ComPtr<IFileSaveDialog> pfsd;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        WCHAR szFullPathToTestFile[MAX_PATH] = {};

        // For this exercise, let's just support only one file type to make things simpler.
        // Also, let's use the jpg format for sample purpose because the Windows ships with
        // property handlers for jpg files.
        const COMDLG_FILTERSPEC rgSaveTypes[] = {{L"Photo Document (*.jpg)", L"*.jpg"}};

        // Set the file types to display.
        hr = pfsd->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);
        if (SUCCEEDED(hr))
        {
            hr = pfsd->SetFileTypeIndex(0);
            if (SUCCEEDED(hr))
            {
                // Set default file extension.
                hr = pfsd->SetDefaultExtension(L"jpg");
                if (SUCCEEDED(hr))
                {
                    // Ensure the dialog only returns items that can be represented by file system paths.
                    DWORD dwFlags;
                    hr = pfsd->GetOptions(&dwFlags);
                    if (SUCCEEDED(hr))
                    {
                        // Let's first get the current property set of the file we are replicating
                        // and give it to the file dialog object.
                        //
                        // For simplicity sake, let's just get the property set from a pre-existing jpg file
                        // (in the Pictures folder). In the real-world, you would actually add code to get
                        // the property set of the file that is currently open and is being replicated.
                        hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                        if (SUCCEEDED(hr))
                        {
                            // Attempt to retrieve the path to %PUBLIC%\Pictures\Sample Pictures.
                            // The folder must exist for this to succeed.
                            PWSTR pszPicturesFolderPath;
                            hr = SHGetKnownFolderPath(FOLDERID_SamplePictures, 0, nullptr, &pszPicturesFolderPath);
                            if (SUCCEEDED(hr))
                            {
                                hr = PathCombineW(szFullPathToTestFile, pszPicturesFolderPath, L"Flower.jpg") ? S_OK : E_FAIL;
                                if (SUCCEEDED(hr))
                                {
                                    ComPtr<IPropertyStore> pps;
                                    hr = SHGetPropertyStoreFromParsingName(szFullPathToTestFile, nullptr, GPS_DEFAULT, IID_PPV_ARGS(&pps));
                                    if (FAILED(hr))
                                    {
                                        // Flower.jpg is probably not in the Pictures folder.
                                        TaskDialog(nullptr, nullptr, L"CommonFileDialogApp", L"Create Flower.jpg in the Pictures folder and try again.",
                                                   nullptr, TDCBF_OK_BUTTON, TD_ERROR_ICON , nullptr);
                                    }
                                    else
                                    {
                                        // Call SetProperties on the file dialog object for getting back later.
                                        pfsd->SetCollectedProperties(nullptr, TRUE);
                                        pfsd->SetProperties(pps.Get());
                                    }
                                }
                                CoTaskMemFree(pszPicturesFolderPath);
                            }
                        }
                    }
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = pfsd->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                ComPtr<IShellItem> psiResult;
                hr = pfsd->GetResult(&psiResult);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszNewFileName;
                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszNewFileName);
                    if (SUCCEEDED(hr))
                    {
                        // This is where you add code to write data to your file.
                        // For simplicity, let's just copy a pre-existing dummy jpg file.
                        //
                        // In the real-world, you would actually add code to replicate the data of
                        // file that is currently open.
                        hr = CopyFileW(szFullPathToTestFile, pszNewFileName, FALSE) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
                        if (SUCCEEDED(hr))
                        {
                            // Now apply the properties.
                            //
                            // Get the property store first by calling GetPropertyStore and pass it on to ApplyProperties.
                            // This will make the registered propety handler for the specified file type (jpg)
                            // do all the work of writing the properties for you.
                            //
                            // Property handlers for the specified file type should be registered for this
                            // to work.
                            ComPtr<IPropertyStore> pps;

                            // When we call GetProperties, we get back all the properties that we originally set
                            // (in our call to SetProperties above) plus the ones user modified in the file dialog.
                            hr = pfsd->GetProperties(&pps);
                            if (SUCCEEDED(hr))
                            {
                                // Now apply the properties making use of the registered property handler for the file type.
                                //
                                // hWnd is used as parent for any error dialogs that might popup when writing properties.
                                // Pass nullptr for IFileOperationProgressSink as we don't want to register any callback
                                // for progress notifications.
                                hr = pfsd->ApplyProperties(psiResult.Get(), pps.Get(), nullptr, nullptr);
                            }
                        }
                        CoTaskMemFree(pszNewFileName);
                    }
                }
            }
        }
    }
    return hr;
}

// This code snippet demonstrates how to write properties without using property handlers.
HRESULT WritePropertiesWithoutUsingHandlers()
{
    // CoCreate the File Save Dialog object.
    ComPtr<IFileSaveDialog> pfsd;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));
    if (SUCCEEDED(hr))
    {
        // For this exercise, let's use a custom file type.
        const COMDLG_FILTERSPEC rgSaveTypes[] = {{L"MyApp Document (*.myApp)", L"*.myApp"}};

        // Set the file types to display.
        hr = pfsd->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);
        if (SUCCEEDED(hr))
        {
            hr = pfsd->SetFileTypeIndex(0);
            if (SUCCEEDED(hr))
            {
                // Set default file extension.
                hr = pfsd->SetDefaultExtension(L"myApp");
                if (SUCCEEDED(hr))
                {
                    // Ensure the dialog only returns items that can be represented by file system paths.
                    DWORD dwFlags;
                    hr = pfsd->GetOptions(&dwFlags);
                    if (SUCCEEDED(hr))
                    {
                        hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                        if (SUCCEEDED(hr))
                        {
                            // Set the properties you want the FileSave dialog to collect from the user.
                            ComPtr<IPropertyDescriptionList> pdl;
                            hr = PSGetPropertyDescriptionListFromString(L"prop:System.Keywords", IID_PPV_ARGS(&pdl));
                            if (SUCCEEDED(hr))
                            {
                                // TRUE as second param == show default properties as well, but show Keyword first.
                                hr = pfsd->SetCollectedProperties(pdl.Get(), TRUE);
                            }
                        }
                    }
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            // Now show the dialog.
            hr = pfsd->Show(nullptr);
            if (SUCCEEDED(hr))
            {
                ComPtr<IShellItem> psiResult;
                hr = pfsd->GetResult(&psiResult);
                if (SUCCEEDED(hr))
                {
                    // Get the path to the file.
                    PWSTR pszNewFileName;
                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszNewFileName);
                    if (SUCCEEDED(hr))
                    {
                        // Write data to the file.
                        WriteDataToCustomFile(pszNewFileName);

                        // Now get the property store and write each individual property to the file.
                        ComPtr<IPropertyStore> pps;
                        hr = pfsd->GetProperties(&pps);
                        if (SUCCEEDED(hr))
                        {
                            // Add some properties manually.
                            AddPropertyToStore(pps.Get(), PKEY_Author, L"Some Author");
                            AddPropertyToStore(pps.Get(), PKEY_Comment, L"This is a comment");

                            // Loop over property set and write each property/value pair to the file.
                            DWORD cProps = 0;
                            hr = pps->GetCount(&cProps);
                            for (DWORD i = 0; i < cProps && SUCCEEDED(hr); i++)
                            {
                                PROPERTYKEY key;
                                hr = pps->GetAt(i, &key);
                                if (SUCCEEDED(hr))
                                {
                                    PWSTR pszPropertyName;
                                    hr = PSGetNameFromPropertyKey(key, &pszPropertyName);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Get the value of the property.
                                        PROPVARIANT propvarValue;
                                        PropVariantInit(&propvarValue);
                                        hr = pps->GetValue(key, &propvarValue);
                                        if (SUCCEEDED(hr))
                                        {
                                            WCHAR wszValue[MAX_PATH];

                                            // Always use property system APIs to do the conversion for you.
                                            hr = PropVariantToString(propvarValue, wszValue, ARRAYSIZE(wszValue));
                                            if (SUCCEEDED(hr))
                                            {
                                                // Append the property to the file.
                                                AppendPropertyToCustomFile(pszNewFileName, pszPropertyName, wszValue);
                                            }
                                        }
                                        PropVariantClear(&propvarValue);
                                        CoTaskMemFree(pszPropertyName);
                                    }
                                }
                            }
                        }

                        CoTaskMemFree(pszNewFileName);
                    }
                }
            }
        }
    }
    return hr;
}

