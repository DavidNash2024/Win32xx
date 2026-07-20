#include "pch.h"
#include <iostream>  // defines std::wcout

bool RunCreateKeyTest(CRegKey& key)
{
    const LPCTSTR TEST_KEY_PATH = _T("Software\\Win32xx_Registry_Test");

    // Test Creation.
    LONG status = key.Create(HKEY_CURRENT_USER, TEST_KEY_PATH);

    return (status == ERROR_SUCCESS);
}


// Dynamic String Set & Query.
bool RunStringTest(CRegKey& workerKey)
{
    CString inputStr = _T("Test User XP");
    CString outputStr = _T("Test User XP");
    LONG status = workerKey.SetStringValue(_T("Username"), inputStr);
    assert(status == ERROR_SUCCESS);
    bool pass = (status == ERROR_SUCCESS);

    CString value;
    status = workerKey.QueryStringValue(_T("Username"), value);
    pass = (status == ERROR_SUCCESS);
    pass = pass && (inputStr == outputStr);

    return pass;
}

// Multi-String Vector Set & Query.
bool RunMultiStringTest(CRegKey& workerKey)
{
    std::vector<CString> input = { "Alpha", "Beta", "Gamma" };
    LONG status = workerKey.SetMultiStringValue(_T("ConfigList"), input);
    bool pass = (status == ERROR_SUCCESS);

    std::vector<CString> output;
    status = workerKey.QueryMultiStringValue(_T("ConfigList"), output);
    pass = pass && (input.size() == output.size());
    pass = pass && (input == output);
    pass = pass && (status == ERROR_SUCCESS);
    return true;
}

// Query and set value.
bool RunValueTests(CRegKey& workerKey)
{
    BOOL BOOLValue = TRUE;
    bool boolValue = true;
    DWORD dwordValue = 256;
    ULONGLONG qwordValue = 0xFFFFFFFFFFFF;

    LONG status = workerKey.SetBOOLValue(_T("BOOL value"), BOOLValue);
    bool pass = (status == ERROR_SUCCESS);
    status = workerKey.SetBoolValue(_T("bool value"), boolValue);
    pass = pass && (status == ERROR_SUCCESS);
    status = workerKey.SetDWORDValue(_T("DWORD value"), dwordValue);
    pass = pass && (status == ERROR_SUCCESS);
    status = workerKey.SetQWORDValue(_T("QWORD value"), qwordValue);
    pass = pass && (status == ERROR_SUCCESS);

    status = workerKey.QueryBOOLValue(_T("BOOL value"), BOOLValue);
    pass = pass && (status == ERROR_SUCCESS);
    pass = pass && (BOOLValue == TRUE);
    status = workerKey.QueryBoolValue(_T("bool value"), boolValue);
    pass = pass && (status == ERROR_SUCCESS);
    pass = pass && (boolValue == true);
    status = workerKey.QueryDWORDValue(_T("DWORD value"), dwordValue);
    pass = pass && (status == ERROR_SUCCESS);
    pass = pass && (dwordValue == 256);
    status = workerKey.QueryQWORDValue(_T("QWORD Value"), qwordValue);
    pass = pass && (status == ERROR_SUCCESS);
    pass = pass && (qwordValue == 0xFFFFFFFFFFFF);

    return pass;
}

// GUID Validation.
bool RunGUIDTest(CRegKey& workerKey)
{
    GUID inputGuid = { 0x3f10ef40, 0x2b3e, 0x11d3, { 0x9b, 0xa5, 0x0, 0xc0, 0x4f, 0x8e, 0xf6, 0xc3 } };
    GUID outputGuid = GUID_NULL;
    LONG status = workerKey.SetGUIDValue(_T("AppID"), inputGuid);
    bool pass = (status == ERROR_SUCCESS);

    status = workerKey.QueryGUIDValue(_T("AppID"), outputGuid);
    pass = pass && (status == ERROR_SUCCESS);
    pass = pass && (::IsEqualGUID(inputGuid, outputGuid) == TRUE);

    return true;
}

bool RunRecurseDelete()
{
    CRegKey cleanupKey;
    LONG status = cleanupKey.Open(HKEY_CURRENT_USER, _T("Software"));
    if (status == ERROR_SUCCESS)
        status = cleanupKey.RecurseDeleteKey(_T("Win32xx_Registry_Test"));

    return (status == ERROR_SUCCESS);
}

void RunRegistryTests()
{
    CRegKey key;
    using std::cout;
    cout << "Create key test            "; RunCreateKeyTest(key) ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Query and set string test  "; RunStringTest(key) ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Multi string test          "; RunMultiStringTest(key) ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Query and set values       "; RunValueTests(key) ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GUID test                  "; RunGUIDTest(key) ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Recurse delete test        "; RunRecurseDelete() ? cout << ("Passed\n") : cout << ("***Failed***\n");
}

int main()
{
    RunRegistryTests();
    return 0;
}