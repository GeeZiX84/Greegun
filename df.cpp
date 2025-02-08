#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <sstream>
#include <thread>
#include "wind.h"
#include "ssh.h"

using namespace std;

void sshTask() {
    const char *i = "-";
    const char *u = "-";
    const char *p = "-";
    Connect connection(i, u, p);

    if (connection.getSession() != NULL) {
        connection.interactive_shell(connection.getSession());
    } else {
        MessageBox(NULL, L"Ошибка создания SSH-сессии!", L"Ошибка", MB_OK | MB_ICONERROR);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Windows app(hInstance, nCmdShow);

    // Создаем отдельный поток для обработки SSH-подключения
    std::thread sshThread(sshTask);

    app.Run(); // Это запустит цикл событий для GUI

    // Ожидаем завершения потока SSH перед выходом
    sshThread.join();

    return 0;
}