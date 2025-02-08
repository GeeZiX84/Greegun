#pragma once
#include <windows.h>
#include <string>
#include "UserData.h"

using namespace std;

class Windows {
public:
    Windows(HINSTANCE hInstance, int nCmdShow);
    void Run();

private:
    HINSTANCE hInst;
    HWND hUsername1, hUsername2, hPassword, hButton, hSaveButton, hSaveCheckBox;
    UserData userData;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void RegisterWindowClass();
    HWND CreateMainWindow();
    void MainLoop();
};