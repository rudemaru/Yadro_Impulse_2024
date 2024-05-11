#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <locale>
#include <regex>
#include <map>
#include <set>
#include <vector>
#include "./utils/structs/client_data.hpp"
#include "./utils/structs/club_data.hpp"
#include "./utils/handlers/file_handler.hpp"
#include "./utils/handlers/action_handler.hpp"

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "rus");

    std::string filePath;

    if(argc > 1){
        filePath = argv[1];
    }else{
        std::cout<<"������������ ���������� (������� ���� � �����).\n";
        exit(0);
    }

    ClubData clubData;

    FileHandler::validCheck(clubData, filePath);

    std::map<int, ClientData*> tableMap;
    std::map<std::string, int> clientMap;
    std::vector<int> freeTables;
    
    for(int i = 1; i <= clubData.tables_count; i++){
        tableMap.insert(std::make_pair(i, nullptr));
        freeTables.push_back(i);
    }
    std::vector<std::string> clientQueue;
    
    ClubMap clubMap = {tableMap, clientMap, freeTables, clientQueue};

    ActionHandler ah = ActionHandler(clubData, clubMap);

    ah.run(clubData, filePath);

    return 0;
}