#pragma once
#include <libssh/libssh.h>
class Connect
{
  public:
    Connect(const char *i,const char *u,const char *p);
    void interactive_shell(ssh_session session);
    void file(ssh_session session);
    
  private:
    const char *ip_adress;
    const char *username;
    const char *password;
    ssh_session session; 
};