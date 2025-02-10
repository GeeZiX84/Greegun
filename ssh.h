#pragma once
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <string>
class Connect {
public:
    Connect(const char* i, const char* u, const char* p);
    ssh_session getSession(); // Single declaration of getSession
    sftp_session getSftpSession();
    void interactive_shell(ssh_session session);
    void transferFile(ssh_session session, std::string localPath, std::string remotePath);
private:
    const char *ip_address;
    const char *username;
    const char *password;
    std::string localPath;
    std::string remotePath;
    ssh_session session;
    sftp_session sftp;
};
//.