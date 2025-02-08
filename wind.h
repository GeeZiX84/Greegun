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
    // Глобальные переменные
    HINSTANCE hInst;
    HWND hUsername1, hUsername2, hPassword, hButton;
    UserData userData; // Создание объекта класса UserData

    // Функция обработки окна
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Функция регистрации класса окна
    void RegisterWindowClass();

    // Функция создания окна
    HWND CreateMainWindow();

    // Функция запуска основного цикла сообщений
    void MainLoop();
};