#include "wind.h"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Windows app(hInstance, nCmdShow);
    app.Run();

    return 0;
}