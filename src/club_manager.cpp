#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <locale>
#include <regex>
#include "./utils/structs/club_data.hpp"
#include "./utils/handlers/file_handler.hpp"

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "rus");

    std::string filePath;

    if(argc > 1){
        filePath = argv[1];
    }else{
        std::cout<<"Недостаточно аргументов (Укажите путь к файлу).\n";
        exit(0);
    }

    ClubData clubData;

    FileHandler::validCheck(clubData, filePath);

    std::cout<<"tables_count: "<<clubData.tables_count<<std::endl;
    std::cout<<"open_time: "<<clubData.open_time<<"_\n"; // "_" чтобы видеть или не были захвачены лишние пробелы
    std::cout<<"close_time: "<<clubData.close_time<<"_\n";
    std::cout<<"price_per_hour: "<<clubData.price_per_hour<<std::endl;

    return 0;
}