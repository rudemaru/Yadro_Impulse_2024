#ifndef _CLUB_MAP_
#define _CLUB_MAP_

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include "client_data.hpp"

typedef struct{
    //мапа номер места - юзер на этом месте
    std::map<int, ClientData>& tableMap;
    //мапа юзер - номер места (если он занимает место)
    std::map<std::string, int> clientMap;
    //пул свободных столов
    std::vector<int> freeTables;
    //очередь клиентов в ожидании
    std::queue<ClientData>& clientQueue;
}ClubMap;

#endif