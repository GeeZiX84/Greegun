#include <libssh/libssh.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>

void interactive_shell(ssh_session session) {
    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) {
        std::cerr << "Ошибка создания SSH-канала!\n";
        return;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        std::cerr << "Ошибка открытия канала: " << ssh_get_error(session) << "\n";
        ssh_channel_free(channel);
        return;
    }

    if (ssh_channel_request_pty(channel) != SSH_OK) {
        std::cerr << "Ошибка запроса PTY: " << ssh_get_error(session) << "\n";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    if (ssh_channel_request_shell(channel) != SSH_OK) {
        std::cerr << "Ошибка запуска shell: " << ssh_get_error(session) << "\n";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    std::cout << "=== Подключено к SSH. Введите команды (exit для выхода) ===\n";

    char buffer[256];
    std::string user_input;

    while (ssh_channel_is_open(channel) && !ssh_channel_is_eof(channel)) {
        if (_kbhit()) {  // Проверяем, есть ли ввод от пользователя (Windows)
            std::getline(std::cin, user_input);
            if (user_input == "exit") break;
            else if(user_input != "ls"){
                std::cout << "Введите ls типо для запуска сервера" << std::endl;
            }
            else{
                user_input += "\n";
                ssh_channel_write(channel, user_input.c_str(), user_input.size());
            }
        }

        // Проверяем, есть ли данные для чтения с сервера
        if (ssh_channel_poll(channel, 0) > 0) {
            memset(buffer, 0, sizeof(buffer));
            int n = ssh_channel_read(channel, buffer, sizeof(buffer) - 1, 0);
            if (n > 0) {
                std::cout << buffer;
                std::cout.flush();
            }
        }
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

int main() {
    ssh_session session = ssh_new();
    if (session == NULL) {
        std::cerr << "Ошибка создания SSH-сессии!\n";
        return 1;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "193.34.213.252");
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    if (ssh_connect(session) != SSH_OK) {
        std::cerr << "Ошибка подключения: " << ssh_get_error(session) << "\n";
        ssh_free(session);
        return 1;
    }

    if (ssh_userauth_password(session, "root", "&DrS5w$rz100") != SSH_AUTH_SUCCESS) {
        std::cerr << "Ошибка аутентификации: " << ssh_get_error(session) << "\n";
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    std::cout << "Успешное подключение к серверу!\n";
    interactive_shell(session);

    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}
