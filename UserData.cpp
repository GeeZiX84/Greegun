#include "UserData.h"
#include <fstream>

void UserData::LoadData() {
    // Инициализация значениями по умолчанию
    username1 = L"";
    username2 = L"";
    password = L"";
    
    wifstream infile("userdata.txt");
    if (infile.is_open()) {
        // Считываем данные из файла
        getline(infile, username1);
        getline(infile, username2);
        getline(infile, password);
        infile.close();
    }
}

void UserData::SaveData() {
    // Открываем файл на запись
    wofstream outfile("userdata.txt");
    if (outfile.is_open()) {
        // Записываем данные в файл
        outfile << username1 << endl;
        outfile << username2 << endl;
        outfile << password << endl;
        outfile.close();
    }
}
//.