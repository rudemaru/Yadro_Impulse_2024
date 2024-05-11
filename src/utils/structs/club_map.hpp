#ifndef _CLUB_MAP_
#define _CLUB_MAP_

#include <map>
#include <vector>
#include "client_data.hpp"

struct ClubMap{
    //мапа номер места - юзер на этом месте
    std::map<int, ClientData*> tableMap;
    //мапа юзер - номер места (если он занимает место)
    std::map<std::string, int> clientMap;
    //пул свободных столов
    std::vector<int> freeTables;
    //очередь клиентов в ожидании
    std::vector<std::string> clientQueue;
};

#endif