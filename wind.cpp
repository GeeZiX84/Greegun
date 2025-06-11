#include "ssh.h"
#include "UserData.h"
#include "wind.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <sstream>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h> 
#include <richedit.h>
using namespace std;

std::wstring OpenFileDialog() {
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"Config Files\0*.cfg\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return std::wstring(szFile);
    } else {
        return L"";
    }
}
std::string OpenFileDialogWrapper() {
    std::wstring wfilePath = OpenFileDialog();
    if (!wfilePath.empty()) {
        return std::string(wfilePath.begin(), wfilePath.end());
    } else {
        return "";
    }
}
void Windows::ExecuteCommand(const std::wstring& command) {
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    CreatePipe(&hRead, &hWrite, &sa, 0);
    
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.wShowWindow = SW_HIDE;
    
    PROCESS_INFORMATION pi;
    if (CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hWrite);
        
        wchar_t buffer[4096];
        DWORD bytesRead;
        std::wstring output;
        while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead / sizeof(wchar_t)] = 0;
            output += buffer;
        }
        
        CloseHandle(hRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        AppendConsoleText(output + L"\n");
    } else {
        AppendConsoleText(L"Ошибка запуска команды!\n");
    }
}
Windows::Windows(HINSTANCE hInstance, int nCmdShow) : hInst(hInstance), connection(nullptr) {
    RegisterWindowClass();

    HWND hWnd = CreateMainWindow();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MainLoop();
}

void Windows::Run() {
    // Запуск приложения
}

void Windows::RegisterWindowClass() {
    LoadLibrary(TEXT("Msftedit.dll"));
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = Windows::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"LoginWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
}

void Windows::AppendConsoleText(const std::wstring& text) {
    int len = GetWindowTextLengthW(hConsole);
    SendMessageW(hConsole, EM_SETSEL, (WPARAM)len, (LPARAM)len);
    SendMessageW(hConsole, EM_REPLACESEL, 0, (LPARAM)text.c_str());
}

HWND Windows::CreateMainWindow() {
    return CreateWindowW(L"LoginWindow", L"Авторизация", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1280 , 800, NULL, NULL, hInst, this);
}

void Windows::MainLoop() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Windows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Windows* app = reinterpret_cast<Windows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        app = reinterpret_cast<Windows*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }

    if (app) {
        
        switch (message) {
            case WM_CREATE: {
                app->userData.LoadData();
                
                CreateWindowW(L"STATIC", L"IP", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hIpAddress = CreateWindowW(L"EDIT",app->userData.username2.c_str() , WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 20, 150, 20, hWnd, NULL, app->hInst, NULL);

                CreateWindowW(L"STATIC", L"Username", WS_VISIBLE | WS_CHILD, 20, 50, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hUsername1 = CreateWindowW(L"EDIT", app->userData.username1.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 50, 150, 20, hWnd, NULL, app->hInst, NULL);

                CreateWindowW(L"STATIC", L"Пароль:", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hPassword = CreateWindowW(L"EDIT", app->userData.password.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 130, 80, 150, 20, hWnd, NULL, app->hInst, NULL);

                app->hButton = CreateWindowW(L"BUTTON", L"Войти", WS_VISIBLE | WS_CHILD, 130, 110, 150, 30, hWnd, (HMENU)1, app->hInst, NULL);
                app->hSaveCheckBox = CreateWindowW(L"BUTTON", L"Сохранить", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 130, 150, 150, 20, hWnd, (HMENU)2, app->hInst, NULL);
                app->hConnectButton = CreateWindowW(L"BUTTON", L"Подключиться", WS_VISIBLE | WS_CHILD, 130, 180, 150, 30, hWnd, (HMENU)3, app->hInst, NULL);
                
                // Add a button to open the file dialog
                CreateWindowW(L"BUTTON", L"Выбрать файл", WS_VISIBLE | WS_CHILD, 130, 220, 150, 30, hWnd, (HMENU)4, app->hInst, NULL);

                LoadLibrary(TEXT("Msftedit.dll")); // Загружаем библиотеку RichEdit

                app->hConsole = CreateWindowExW(WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, L"", 
                        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 
                        20, 300, 800, 400, hWnd, NULL, app->hInst, NULL);
                
                app->hConsoleInput = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 
                        20, 710, 340, 25, hWnd, NULL, app->hInst, NULL);
                CreateWindowW(L"BUTTON", L"Отправить", WS_VISIBLE | WS_CHILD, 370, 710, 90, 25, hWnd, (HMENU)5, app->hInst, NULL);
                
            }
            break;
            case WM_COMMAND:

                if (LOWORD(wParam) == 1) {  
                    wchar_t username1[50], password[50], username2[50];
                    GetWindowTextW(app->hUsername2, username2, 50);
                    GetWindowTextW(app->hUsername1, username1, 50);
                    GetWindowTextW(app->hPassword, password, 50);

                    app->userData.username1 = username1;
                    app->userData.password = password;
                    app -> userData.username2 = username2;


                    if (SendMessage(app->hSaveCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                        app->userData.SaveData();
                        MessageBoxW(hWnd, L"Данные сохранены", L"Сохранение", MB_OK | MB_ICONINFORMATION);
                    }

                    wchar_t message[200];
                    wsprintfW(message, L"Юзернейм: %s\nПароль: %s", username1, password);
                    MessageBoxW(hWnd, message, L"Вход выполнен", MB_OK | MB_ICONINFORMATION);

                
                } else if (LOWORD(wParam) == 4) 
                { // Handle file dialog button click
                    std::wstring filePath = OpenFileDialog();
                    if (!filePath.empty()) {
                        std::wstring wFilePath(filePath.begin(), filePath.end());
                        
                    

                    char ip[50];
                    wcstombs(ip, app->userData.username2.c_str(), 50);

                    char username[50];
                    wcstombs(username, app->userData.username1.c_str(), 50);

                    char password[50];
                    wcstombs(password, app->userData.password.c_str(), 50);

                    if (!app->connection) {
                        delete app->connection;
                    }

                    app->connection = new Connect(ip, username, password);
                    char Path[100];
                    wcstombs(Path, wFilePath.c_str(), 100);
                    if (app->connection->getSession() != nullptr) {
                        MessageBoxW(hWnd, L"Успешно подключено к серверу!", L"Подключение", MB_OK | MB_ICONINFORMATION);
                    }   
                    else {
                            MessageBoxW(hWnd, L"Ошибка подключения к серверу.", L"Ошибка", MB_OK | MB_ICONERROR);
                        }
                        app->connection->send_file(app->connection->getSession(),string(Path),"/root/moe"); 
                        MessageBoxW(hWnd, wFilePath.c_str(), L"Выбранный файл", MB_OK | MB_ICONINFORMATION);

                    } else {
                        MessageBoxW(hWnd, L"Файл не выбран.", L"Ошибка", MB_OK | MB_ICONERROR);
                    }
                }
            
                else if (LOWORD(wParam) == 5) {
                    wchar_t input[256];
                    GetWindowTextW(app->hConsoleInput, input, 256);
                    if (wcslen(input) > 0) {
                        app->AppendConsoleText(L"\n> " + std::wstring(input) + L"\n");
                        app->ExecuteCommand(input);
                    }
                    SetWindowTextW(app->hConsoleInput, L"");
                }
                break;
            case WM_DESTROY:
                
                
                DestroyWindow(app->hButton);
                DestroyWindow(app->hSaveCheckBox);
                DestroyWindow(app->hConnectButton);
                DestroyWindow(app->hIpAddress);
                PostQuitMessage(0);
                break;
            default:
                return DefWindowProcW(hWnd, message, wParam, lParam);
        }
    } 
        return DefWindowProcW(hWnd, message, wParam, lParam);
    return 0;
}
