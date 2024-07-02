// MacroRun.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

// Funktion zum Überprüfen, ob eine Datei existiert
bool FileExists(const std::wstring& filePath) {
    std::ifstream file(filePath.c_str());
    return file.good();
}

// Funktion zum Simulieren eines Tastendrucks
void PressKey(WORD key, bool hold = false) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));

    if (!hold) {
        // Taste loslassen
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

// Funktion zum Loslassen einer Taste
void ReleaseKey(WORD key) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

// Funktion zum Simulieren der Eingabe eines Textes
void TypeText(const std::wstring& text) {
    for (size_t i = 0; i < text.size(); ++i) {
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = text[i];
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input, sizeof(INPUT));

        // Taste loslassen
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

// Funktion zum Senden des Ausführen-Befehls mit Argumenten
void SendRunCommand(const std::wstring& programPath, const std::wstring& arguments = L"") {
    std::wstring command = L"\"" + programPath + L"\"";
    if (!arguments.empty()) {
        command += L" " + arguments;
    }

    // Überprüfen, ob die Datei existiert
    if (!FileExists(programPath)) {
        std::wcerr << L"File not found: " << programPath << std::endl;
        return;
    }

    // Ausführen-Dialog öffnen (Win + R)
    PressKey(VK_LWIN, true); // Halte Win gedrückt
    PressKey('R');
    ReleaseKey(VK_LWIN); // Lass Win los

    // Warten, bis der Ausführen-Dialog geöffnet ist
    Sleep(500);

    // Text eingeben
    TypeText(command);

    // Warten, um sicherzustellen, dass der Text eingegeben wird
    Sleep(500);

    // Enter-Taste drücken, um das Kommando auszuführen
    PressKey(VK_RETURN);

    // Warten, um sicherzustellen, dass das Programm gestartet wird
    Sleep(1000);
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcerr << L"Usage: " << argv[0] << L" <path_to_program> [arguments]" << std::endl;
        return 1;
    }

    std::wstring programPath = argv[1];

    // Überprüfen, ob die Datei existiert
    if (!FileExists(programPath)) {
        std::wcerr << L"File not found: " << programPath << std::endl;
        return 1;
    }

    // Sammeln der Argumente (falls vorhanden)
    std::wstring arguments;
    for (int i = 2; i < argc; ++i) {
        arguments += argv[i];
        if (i < argc - 1) {
            arguments += L" ";
        }
    }

    // Senden des Befehls an den Ausführen-Dialog
    SendRunCommand(programPath, arguments);

    return 0;
}