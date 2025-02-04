#include <iostream>
#include <libssh/libssh.h>

int main() {
    std::cout << "Программа запустилась!\n";

    ssh_session my_ssh_session = ssh_new();
    if (my_ssh_session == nullptr) {
        std::cerr << "Ошибка создания SSH-сессии!\n";
        return 1;
    }

    std::cout << "SSH-сессия создана успешно!\n";
    ssh_free(my_ssh_session);

    return 0;
}
