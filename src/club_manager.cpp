#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <locale>
#include <regex>
#include "./utils/club_data.hpp"
#include "./utils/file_handler.hpp"

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "rus");

    std::string file_path;

    if(argc > 1){
        file_path = argv[1];
    }else{
        std::cout<<"Недостаточно аргументов (Укажите путь к файлу).\n";
        exit(0);
    }

    ClubData cd;

    FileHandler fh = FileHandler(file_path);

    fh.validCheck(cd);

    return 0;
}