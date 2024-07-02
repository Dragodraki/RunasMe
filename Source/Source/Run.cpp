#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include <shellapi.h>

// Funktion zur Ausgabe der Benutzungsinformationen
void PrintUsage() {
    MessageBox(NULL,
        L"Usage: ProgramLauncher.exe /UACMode /WaitMode <PathToProgram> [ProgramArguments...]\n"
        L"UACMode: /inherit | /force\n"
        L"WaitMode: /wait | /nowait\n",
        L"Usage Information",
        MB_OK | MB_ICONINFORMATION);
}

// Funktion zur Zusammenführung der Programarguments in eine Zeichenkette
std::wstring JoinArguments(int argc, _TCHAR* argv[], int start) {
    std::wstring args;
    for (int i = start; i < argc; ++i) {
        if (i > start) {
            args += L" ";
        }
        args += argv[i];
    }
    return args;
}

// Der Einstiegspunkt der Windows-Anwendung
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc < 4) {
        PrintUsage();
        LocalFree(argv);
        return 1;
    }

    const wchar_t* uacMode = argv[1];
    const wchar_t* waitMode = argv[2];
    const wchar_t* programPath = argv[3];
    std::wstring programArgs;

    if (argc > 4) {
        programArgs = JoinArguments(argc, argv, 4);
    }

    if ((_wcsicmp(uacMode, L"/inherit") != 0 && _wcsicmp(uacMode, L"/force") != 0) ||
        (_wcsicmp(waitMode, L"/wait") != 0 && _wcsicmp(waitMode, L"/nowait") != 0)) {
        PrintUsage();
        LocalFree(argv);
        return 1;
    }

    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpFile = programPath;
    sei.lpParameters = programArgs.empty() ? NULL : programArgs.c_str();
    sei.nShow = SW_SHOWNORMAL;

    if (_wcsicmp(uacMode, L"/force") == 0) {
        sei.lpVerb = L"runas";  // Force UAC prompt
        if (!ShellExecuteEx(&sei)) {
            //MessageBox(NULL, L"Error: Unable to start program with UAC.", L"Error", MB_OK | MB_ICONERROR);
            LocalFree(argv);
            return 1;
        }
    } else if (_wcsicmp(uacMode, L"/inherit") == 0) {
        sei.lpVerb = NULL;  // Inherit UAC from manifest
        if (!ShellExecuteEx(&sei)) {
            //MessageBox(NULL, L"Error: Unable to start program.", L"Error", MB_OK | MB_ICONERROR);
            LocalFree(argv);
            return 1;
        }
    }

    if (_wcsicmp(waitMode, L"/wait") == 0) {
        // Wait until the launched program exits
        WaitForSingleObject(sei.hProcess, INFINITE);
    } else {
        // /nowait: Do not wait, exit immediately
    }

    CloseHandle(sei.hProcess);
    LocalFree(argv);
    return 0;
}