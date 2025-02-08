#pragma once
#include <string>

using namespace std;

class UserData {
public:
    wstring username1;
    wstring username2;
    wstring password;

    void LoadData();
    void SaveData();
};
