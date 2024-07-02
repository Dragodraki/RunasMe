// Win32ComputerSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <string>
#include <sddl.h>
#include <windows.h>
#include <userenv.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "userenv.lib")

std::wstring GetUserProfilePathFromSID(PSID sid)
{
    LPWSTR sidString = NULL;
    HKEY hKey;
    WCHAR regPath[256];
    DWORD bufferSize = MAX_PATH;
    WCHAR profilePath[MAX_PATH];

    if (ConvertSidToStringSidW(sid, &sidString))
    {
        swprintf_s(regPath, 256, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\%s", sidString);

        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            if (RegQueryValueExW(hKey, L"ProfileImagePath", NULL, NULL, (LPBYTE)profilePath, &bufferSize) == ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                LocalFree(sidString);
                return std::wstring(profilePath);
            }
            RegCloseKey(hKey);
        }
        LocalFree(sidString);
    }
    return L"";
}

int main()
{
    HRESULT hres;

    // Initialize COM.
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize COM library. Error code = 0x"
                  << std::hex << hres << std::endl;
        return 1; // Program has failed.
    }

    // Initialize security.
    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication.
        NULL,                        // Authentication services.
        NULL,                        // Reserved.
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication.
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation.
        NULL,                        // Authentication info.
        EOAC_NONE,                   // Additional capabilities.
        NULL                         // Reserved.
    );

    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize security. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Obtain the initial locator to WMI.
    IWbemLocator *pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *)&pLoc);

    if (FAILED(hres))
    {
        std::cerr << "Failed to create IWbemLocator object. Error code = 0x"
                  << std::hex << hres << std::endl;
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Connect to WMI through the IWbemLocator::ConnectServer method.
    IWbemServices *pSvc = NULL;

    // Connect to the root\cimv2 namespace with the current user.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace.
        NULL,                    // User name. NULL = current user.
        NULL,                    // User password. NULL = current.
        0,                       // Locale. NULL indicates current.
        NULL,                    // Security flags.
        0,                       // Authority (for example, Kerberos).
        0,                       // Context object.
        &pSvc                    // pointer to IWbemServices proxy.
    );

    if (FAILED(hres))
    {
        std::cerr << "Could not connect. Error code = 0x"
                  << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Set security levels on the proxy.
    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set.
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx.
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx.
        NULL,                        // Server principal name.
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx.
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx.
        NULL,                        // Client identity.
        EOAC_NONE                    // Proxy capabilities.
    );

    if (FAILED(hres))
    {
        std::cerr << "Could not set proxy blanket. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Use the IWbemServices pointer to make requests of WMI.
    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT UserName FROM Win32_ComputerSystem"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        std::cerr << "Query for operating system name failed. Error code = 0x"
                  << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Get the data from the query.
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;

        // Get the value of the UserName property.
        hr = pclsObj->Get(L"UserName", 0, &vtProp, 0, 0);
        if (FAILED(hr))
        {
            pclsObj->Release();
            continue;
        }
        std::wcout << "UserName: " << vtProp.bstrVal << std::endl;

        std::wstring userName = vtProp.bstrVal;
        DWORD sidSize = 0;
        DWORD domainSize = 0;
        SID_NAME_USE sidType;
        LookupAccountName(NULL, userName.c_str(), NULL, &sidSize, NULL, &domainSize, &sidType);

        PSID pSid = (PSID)malloc(sidSize);
        if (pSid == NULL)
        {
            VariantClear(&vtProp);
            pclsObj->Release();
            continue;
        }
        LPWSTR domainName = (LPWSTR)malloc(domainSize * sizeof(WCHAR));
        if (domainName == NULL)
        {
            free(pSid);
            VariantClear(&vtProp);
            pclsObj->Release();
            continue;
        }

        if (LookupAccountName(NULL, userName.c_str(), pSid, &sidSize, domainName, &domainSize, &sidType))
        {
            LPWSTR sidString;
            if (ConvertSidToStringSid(pSid, &sidString))
            {
                std::wcout << "SID: " << sidString << std::endl;
                std::wstring profilePath = GetUserProfilePathFromSID(pSid);
                if (!profilePath.empty())
                {
                    std::wcout << "UserProfile: " << profilePath << std::endl;
                }
                else
                {
                    std::wcout << "Failed to retrieve UserProfile path." << std::endl;
                }
                LocalFree(sidString);
            }
        }
        else
        {
            std::wcout << "Failed to lookup account name." << std::endl;
        }

        free(pSid);
        free(domainName);
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup.
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return 0; // Program successfully completed.
}