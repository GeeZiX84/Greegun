#include "ssh.h"
#include <libssh/libssh.h>

int main() {
    const char *io = "-";
    const char *uo = "root";
    const char *po = "-";

    // Создание объекта Connect
    Connect c1(io, uo, po);

    // Вызов метода file с передачей сессии
    c1.file(c1.getSession()); // Передача сессии через метод getSession

    return 0;
}
//.