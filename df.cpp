#include "ssh.h"
int main(){
    const char *io = "-";
    const char *uo = "root";
    const char *po = "-";
    Connect c1{io,uo,po};
    c1.file();
    return 0;
}