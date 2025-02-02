#include "ssh.h"
#include <iostream>
#include <libssh/libssh.h>
Connect::Connect(const char *i,const char *u,const char *p){
    ip_adress = i;
    username = u;
    password = p;
    ssh_session my_ssh_session;
    
}

void Connect::file(){
    printf("File function called\n");
}