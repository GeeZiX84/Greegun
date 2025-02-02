#include "ssh.h"
int main(){
    const char *io = "193.34.213.252";
    const char *uo = "root";
    const char *po = "&DrS5w$rz100";
    Connect c1{io,uo,po};
    c1.file();
    return 0;
}