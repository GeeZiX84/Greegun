#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <list>
#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <conio.h>
#include <winsock2.h>
#include <Windows.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <stdlib.h>

using namespace std;

int main(){
    int i = MessageBox(0, "TEXT", "TEXT", MB_OKCANCEL);
    if (i == IDOK){
        printf("%d\n L");
    }
    else if (i == IDCANCEL){
        printf("%d\n K");
    }
}