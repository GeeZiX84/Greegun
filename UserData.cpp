#include "UserData.h"
#include <fstream>
#include <iostream>

void UserData::LoadData() {
    // Инициализация значениями по умолчанию
    username1 = L"";
    username2 = L"";
    password = L"";
    
    try {
        wifstream infile("userdata.txt");
        if (infile.is_open()) {
            // Считываем данные из файла
            getline(infile, username1);
            getline(infile, username2);
            getline(infile, password);
            infile.close();
        } else {
            std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла: " << e.what() << std::endl;
    }
}

void UserData::SaveData() {
    // Открываем файл на запись
    try {
        wofstream outfile("userdata.txt");
        if (outfile.is_open()) {
            // Записываем данные в файл
            outfile << username1 << std::endl;
            outfile << username2 << std::endl;
            outfile << password << std::endl;
            outfile.close();
            std::cout << "Данные успешно сохранены." << std::endl;
        } else {
            std::cerr << "Не удалось открыть файл для записи." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при записи файла: " << e.what() << std::endl;
    }
}