#include "ssh.h"
#include <libssh/libssh.h>
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <libssh/sftp.h>
#include <fstream>
#include <fcntl.h>       // Include for O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h>    

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
    
    sftp = sftp_new(session);
    if (sftp == NULL) {
        std::cerr << "Ошибка создания SFTP-сессии: " << ssh_get_error(session) << "\n";
        ssh_disconnect(session);
        ssh_free(session);
        session = NULL;
        return;
    }

    if (sftp_init(sftp) != SSH_OK) {
        std::cerr << "Ошибка инициализации SFTP-сессии: " << sftp_get_error(sftp) << "\n";
        sftp_free(sftp);
        ssh_disconnect(session);
        ssh_free(session);
        session = NULL;
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
            } else if (user_input != "ls") {
                std::cout << "Введите ls для запуска сервера" << std::endl;
            } else {
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

void Connect::transferFile(ssh_session session, std::string localPath, std::string remotePath) {
    if (!session || !sftp) {
        std::cerr << "Неверная сессия или SFTP-сессия.\n";
        return;
    }

    std::ifstream file(localPath, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия локального файла: " << localPath << "\n";
        return;
    }

    sftp_file sftpFile = sftp_open(sftp, remotePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (!sftpFile) {
        std::cerr << "Ошибка открытия удаленного файла: " << remotePath << "\n";
        return;
    }

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        if (sftp_write(sftpFile, buffer, file.gcount()) != file.gcount()) {
            std::cerr << "Ошибка записи в удаленный файл.\n";
            sftp_close(sftpFile);
            return;
        }
    }

    sftp_close(sftpFile);
    std::cout << "Файл успешно передан!\n";
}

ssh_session Connect::getSession() {
    return session;
}

sftp_session Connect::getSftpSession() {
    return sftp;
}
