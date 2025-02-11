#pragma once
#include <libssh/libssh.h>
#include <string>
class Connect {
public:
    Connect(const char* i, const char* u, const char* p);
    ssh_session getSession(); // Single declaration of getSession
    void interactive_shell(ssh_session session);
    bool send_file(ssh_session session, std::string local_file, std::string remote_path); 
private:
    const char *ip_address;
    const char *username;
    const char *password;
    std::string local_file;
    std::string remote_path;
    ssh_session session;
};
//.