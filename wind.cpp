#include "wind.h"

Windows::Windows(HINSTANCE hInstance, int nCmdShow) : hInst(hInstance) {
    // Регистрация класса окна
    RegisterWindowClass();

    // Создание главного окна
    HWND hWnd = CreateMainWindow();
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Запуск основного цикла сообщений
    MainLoop();
}

void Windows::Run() {
    // Запуск приложения
}

// Регистрация класса окна
void Windows::RegisterWindowClass() {
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = Windows::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"LoginWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
}

// Создание главного окна
HWND Windows::CreateMainWindow() {
    return CreateWindowW(L"LoginWindow", L"Авторизация", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 
                         CW_USEDEFAULT, CW_USEDEFAULT, 320, 200, NULL, NULL, hInst, NULL);
}

// Основной цикл сообщений
void Windows::MainLoop() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Обработчик оконных сообщений
LRESULT CALLBACK Windows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static Windows* app = nullptr;

    if (message == WM_NCCREATE) {
        app = reinterpret_cast<Windows*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    } else {
        app = reinterpret_cast<Windows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (app) {
        switch (message) {
            case WM_CREATE: {
                app->userData.LoadData();

                CreateWindowW(L"STATIC", L"IP", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hUsername1 = CreateWindowW(L"EDIT", app->userData.username1.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 20, 150, 20, hWnd, NULL, app->hInst, NULL);

                CreateWindowW(L"STATIC", L"Username", WS_VISIBLE | WS_CHILD, 20, 50, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hUsername2 = CreateWindowW(L"EDIT", app->userData.username2.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 50, 150, 20, hWnd, NULL, app->hInst, NULL);

                CreateWindowW(L"STATIC", L"Пароль:", WS_VISIBLE | WS_CHILD, 20, 80, 100, 20, hWnd, NULL, app->hInst, NULL);
                app->hPassword = CreateWindowW(L"EDIT", app->userData.password.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 130, 80, 150, 20, hWnd, NULL, app->hInst, NULL);

                app->hButton = CreateWindowW(L"BUTTON", L"Войти", WS_VISIBLE | WS_CHILD, 130, 110, 150, 30, hWnd, (HMENU)1, app->hInst, NULL);
                break;
            }

            case WM_COMMAND:
                if (LOWORD(wParam) == 1) {  
                    wchar_t username1[50], username2[50], password[50];

                    GetWindowTextW(app->hUsername1, username1, 50);
                    GetWindowTextW(app->hUsername2, username2, 50);
                    GetWindowTextW(app->hPassword, password, 50);

                    app->userData.username1 = username1;
                    app->userData.username2 = username2;
                    app->userData.password = password;

                    app->userData.SaveData();

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
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Windows app(hInstance, nCmdShow);
    app.Run();
    return 0;
}