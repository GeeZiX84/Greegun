#include <Windows.h>

using namespace std;

int main(){
    int i = MessageBox(NULL, "TEXT", "TEXT", MB_OKCANCEL);
    cin.get();
    return i;
}