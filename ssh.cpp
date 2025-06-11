#include "ssh.h"
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <libssh/sftp.h>
#include <fstream>
#include <fcntl.h>       // Include for O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h>   

#define BUFFER_SIZE 8192

Connect::Connect(const char* i, const char* u, const char* p) : ip_address(i), username(u), password(p) {
    session = ssh_new();
    if (session == NULL) {
        std::cerr << "Ошибка создания SSH-сессии!\n";
        return;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, ip_address);
    ssh_options_set(session, SSH_OPTIONS_USER, username);

    if (ssh_connect(session) != SSH_OK) {
        std::cerr << "Ошибка подключения: " << ssh_get_error(session) << "\n";
        ssh_free(session);
        return;
    }

    if (ssh_userauth_password(session, username, password) != SSH_AUTH_SUCCESS) {
        std::cerr << "Ошибка аутентификации: " << ssh_get_error(session) << "\n";
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }
    

    std::cout << "Успешное подключение к серверу!\n";
}

void Connect::interactive_shell(ssh_session session) {
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
            if (user_input == "exit") {
                std::cout << "Выхожу из консоли" << std::endl;
                return;
            }
            else
            {
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

bool Connect::send_file(ssh_session session, std::string local_file, std::string remote_path) {
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path.c_str());
    if (!scp) {
        std::cerr << "Error creating SCP session: " << ssh_get_error(session) << std::endl;
        return false;
    }

    if (ssh_scp_init(scp) != SSH_OK) {
        std::cerr << "Error initializing SCP session: " << ssh_get_error(session) << std::endl;
        ssh_scp_free(scp);
        return false;
    }

    // Read file contents
    std::ifstream file(local_file, std::ios::binary | std::ios::ate);
    std::streamsize file_size = file.tellg();
    if (file_size <= 0) {
        std::cerr << "Error: Invalid file size (" << file_size << ")" << std::endl;
        return false;
    }
    // Push the file to the remote host
    if (ssh_scp_push_file(scp, remote_path.c_str(), file_size, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != SSH_OK) {
        std::cerr << "Error pushing file: " << ssh_get_error(session) << std::endl;
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        return false;
    }

    std::cout << "File successfully transferred!" << std::endl;

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return true;
}


ssh_session Connect::getSession() {
    return session;
}

