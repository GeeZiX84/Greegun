#include <fstream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <windows.h>
#include <list>
#include <cmath>
#include <conio.h>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <stdlib.h>
#include "ssh.h"
using namespace std;
int main(){
    const char *io = "192.168.1.6";
    const char *uo = "user";
    const char *po = "pas";
    Connect Co(io,uo,po);
    Co.cont();
}