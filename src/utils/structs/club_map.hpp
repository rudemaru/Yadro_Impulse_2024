#ifndef _CLUB_MAP_
#define _CLUB_MAP_

#include <map>
#include <vector>
#include <set>
#include "../structs/table_data.hpp"

struct ClubMap{
    std::set<std::string> currentClients;
    //мапа юзер - номер места (если он занимает место)
    std::map<std::string, TableData> clientMap;
    //пул свободных столов
    std::vector<int> freeTables;
    //очередь клиентов в ожидании
    std::vector<std::string> clientQueue;
};

#endif