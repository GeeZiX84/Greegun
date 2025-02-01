#ifndef SSH_H
#define SSH_H
#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;
class Connect{
  private:
    const char *ip_adress;
    const char *username;
    const char *password;
  public:
    Connect(const char *i,const char *u,const char *p);
    void cont();
};
#endif