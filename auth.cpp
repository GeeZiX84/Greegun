#include <windows.h>
#include "UserData.h"
//.
// Использование пространства имен std для удобства
using namespace std;

// Глобальные переменные
HINSTANCE hInst;
HWND hUsername1, hUsername2, hPassword, hButton;
UserData userData; // Создание объекта класса UserData

// Функция обработки окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            // Загрузка данных при создании окна
            userData.LoadData();

            // Создание элементов управления
            CreateWindowW(L"STATIC", L"IP", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hWnd, NULL, hInst, NULL);
            hUsername1 = CreateWindowW(L"EDIT", userData.username1.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 20, 150, 20, hWnd, NULL, hInst, NULL);
            
            CreateWindowW(L"STATIC", L"Username", WS_VISIBLE | WS_CHILD, 20, 50, 100, 20, hWnd, NULL, hInst, NULL);
            hUsername2 = CreateWindowW(L"EDIT", userData.username2.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 50, 150, 20, hWnd, NULL, hInst, NULL);

            // Поле "Пароль"
            CreateWindowW(L"STATIC", L"Пароль:", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hWnd, NULL, hInst, NULL);
            hPassword = CreateWindowW(L"EDIT", userData.password.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 130, 80, 150, 20, hWnd, NULL, hInst, NULL);

            // Кнопка "Войти"
            hButton = CreateWindowW(L"BUTTON", L"Войти", WS_VISIBLE | WS_CHILD, 130, 110, 150, 30, hWnd, (HMENU)1, hInst, NULL);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {  // Нажата кнопка "Войти"
                wchar_t username1[50], username2[50], password[50];

                // Получаем текст из полей ввода
                GetWindowTextW(hUsername1, username1, 50);
                GetWindowTextW(hUsername2, username2, 50);
                GetWindowTextW(hPassword, password, 50);

                // Обновляем данные пользователя
                userData.username1 = username1;
                userData.username2 = username2;
                userData.password = password;

                // Сохраняем данные в файл
                userData.SaveData();

                // Выводим в MessageBox (русский текст поддерживается)
                wchar_t message[200];
                wsprintfW(message, L"Юзернейм 1: %s\nЮзернейм 2: %s\nПароль: %s", username1, username2, password);
                MessageBoxW(hWnd, message, L"Вход выполнен", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Главная функция
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"LoginWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(L"LoginWindow", L"Авторизация", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 
                              CW_USEDEFAULT, CW_USEDEFAULT, 320, 200, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
