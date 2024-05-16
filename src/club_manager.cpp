#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <locale>
#include <regex>
#include <map>
#include <set>
#include <vector>
#include "./utils/structs/table_data.hpp"
#include "./utils/handlers/file_handler.hpp"
#include "./utils/handlers/action_handler.hpp"

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

    std::map<std::string, TableData> clientMap;
    std::vector<int> freeTables;
    std::vector<std::string> clientQueue;
    std::set<std::string> currentClients;
    
    for(int i = 1; i <= clubData.tables_count; i++){
        freeTables.push_back(i);
    }
    
    ClubMap clubMap = {currentClients, clientMap, freeTables, clientQueue};

    ActionHandler ah = ActionHandler(clubData, clubMap);

    ah.run(clubData, filePath);

    return 0;
}