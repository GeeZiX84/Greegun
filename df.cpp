#include "wind.h"
#include "ssh.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <sstream>
#include <thread>

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

    // Create a separate thread to handle SSH connection
    std::thread sshThread(sshTask);

    app.Run(); // This will run the event loop for the GUI

    // Wait for the SSH thread to complete before exiting
    sshThread.join();

    return 0;
}