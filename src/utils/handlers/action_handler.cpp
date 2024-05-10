#include "action_handler.hpp"

ActionHandler::ActionHandler(ClubData cd, ClubMap cm):
    clubData(cd),
    clubMap(cm)
    {}

void ActionHandler::handleCome(const std::string comeTime, const std::string clientName) {
    if(comeTime < clubData.open_time || comeTime >= clubData.close_time){
        throw new Error(comeTime + " 13 NotOpenYet"); 
        return;
    }
    if(clubMap.clientMap.contains(clientName)){
        throw new Error(comeTime + " 13 YouShallNotPass");
        return;
    }
    ClientData clientData = {"", "", clientName, 0};
    clubMap.clientMap.insert(std::make_pair(clientName, 0));
    clubMap.clientQueue.push_back(clientName);
}

void ActionHandler::handleWait(const std::string comeTime, const std::string clientName) {
    if(clubMap.freeTables.size() > 0){
        throw new Error(comeTime + " 13 ICanWaitNoLonger!");
    }
    if(clubMap.clientQueue.size() > clubData.tables_count){
        handleKick(comeTime, clientName);
    }
}

void ActionHandler::handleTakeTable(const std::string comeTime, const std::string clientName, int tableNumber) {
    if(clubMap.clientMap.contains(clientName)){
        if(clubMap.tableMap[tableNumber] == nullptr){
            ClientData *client = new ClientData{comeTime, "", clientName, tableNumber};
            if(clubMap.clientMap[clientName] != 0){
                int current_table = clubMap.clientMap[clientName];
                client = clubMap.tableMap[current_table];

                int session_time = timeInMinutes(comeTime) - timeInMinutes(client->start_time);
                std::cout<<clientName<<" "<<tableNumber<<" "<<session_time<<std::endl;
                clubStat[tableNumber].timeTaken += session_time;
                clubStat[tableNumber].totalProfit += ((session_time)/60)*clubData.price_per_hour;
                if(session_time - (session_time/60)*60 != 0){
                    clubStat[tableNumber].totalProfit += clubData.price_per_hour;
                }

                clubMap.freeTables.push_back(current_table);
                clubMap.tableMap[current_table] = nullptr;
            }
            client->start_time = comeTime;
            clubMap.clientMap[clientName] = tableNumber;
            clubMap.tableMap[tableNumber] = client;

            auto it = std::find(clubMap.freeTables.begin(), clubMap.freeTables.end(), tableNumber);
            if(it != clubMap.freeTables.end()){
                clubMap.freeTables.erase(it);
            }
            clubMap.clientQueue.erase(clubMap.clientQueue.begin());
        }else{
            throw new Error(comeTime + " 13 PlaceIsBusy");
        }
    }else{
        throw new Error(comeTime + " 13 ClientUnknown");
    }
}

void ActionHandler::handleLeft(const std::string comeTime, const std::string clientName){
    if(clubMap.clientMap.contains(clientName)){
        int tableNumber = clubMap.clientMap[clientName];
        std::cout<<tableNumber<<std::endl;
        if(tableNumber != 0 && clubMap.tableMap[tableNumber] != nullptr){
            ClientData *client = clubMap.tableMap[tableNumber]; 
            int session_time = timeInMinutes(comeTime) - timeInMinutes(client->start_time);
            std::cout<<clientName<<" "<<tableNumber<<" "<<session_time<<std::endl;
            clubStat[tableNumber].timeTaken += session_time;
            clubStat[tableNumber].totalProfit += ((session_time)/60)*clubData.price_per_hour;
            if(session_time - (session_time/60)*60 != 0){
                clubStat[tableNumber].totalProfit += clubData.price_per_hour;
            }
            clubMap.tableMap[tableNumber] = nullptr;
            clubMap.freeTables.push_back(tableNumber);
        }
        clubMap.clientMap.erase(clientName);
        auto it = std::find(clubMap.clientQueue.begin(), clubMap.clientQueue.end(), clientName);
        if(it != clubMap.clientQueue.end()){
            clubMap.clientQueue.erase(it);
        }
        handleTakeFree(comeTime);
    }else{
        throw new Error(comeTime + " 13 ClientUnknown");
    }
}

void ActionHandler::handleTakeFree(const std::string comeTime){
    if(!clubMap.clientQueue.empty()){
        std::string clientName = clubMap.clientQueue.front();
        int freeTable = clubMap.freeTables.front();
        ClientData *client = new ClientData{comeTime, "", clientName, freeTable};
        clubMap.tableMap[freeTable] = client;
        clubMap.clientMap[clientName] = freeTable;
        std::cout<<comeTime<<" 12 "<<client->name<<" "<<client->current_table<<std::endl;
        clubMap.clientQueue.erase(clubMap.clientQueue.begin());
        clubMap.freeTables.erase(clubMap.freeTables.begin());
    }else{
        return;
    }
}

void ActionHandler::handleKick(const std::string comeTime, const std::string clientName) {
    
    int tableNumber = clubMap.clientMap[clientName];
    std::cout<<tableNumber<<std::endl;

    if(tableNumber != 0 && clubMap.tableMap[tableNumber] != nullptr){
        ClientData *client = clubMap.tableMap[tableNumber]; 
        int session_time = timeInMinutes(comeTime) - timeInMinutes(client->start_time);
        std::cout<<clientName<<" "<<tableNumber<<" "<<session_time<<std::endl;
        clubStat[tableNumber].timeTaken += session_time;
        clubStat[tableNumber].totalProfit += ((session_time)/60)*clubData.price_per_hour;
        if(session_time - (session_time/60)*60 != 0){
            clubStat[tableNumber].totalProfit += clubData.price_per_hour;
        }
    }
    std::cout<<comeTime<<" "<<11<<" "<<clientName<<std::endl;
}

void ActionHandler::run(ClubData cd, std::string fp){
    std::ifstream file(fp);
    if(file.is_open()){
        std::cout<<clubData.open_time<<std::endl;
        std::string temp_str;
        for(int i = 0; i < 3; i++){
            std::getline(file, temp_str);
        }
        for(int i = 1; i <= cd.tables_count; i++){
            ClubStat cs = {0,0};
            clubStat.insert(std::make_pair(i, cs));
        }
        for(const auto& pair : clubStat){
            std::cout<<pair.first<<" "<<pair.second.totalProfit<<" "<<pair.second.timeTaken<<std::endl;
        }

        int action;

        while(std::getline(file, temp_str)){
            std::cout<<temp_str<<std::endl;
            std::istringstream iss(temp_str);
            action = std::stoi(temp_str.substr(temp_str.find(' ') + 1, temp_str.find(' ') + 2));
            try{
                std::string time;
                std::string name;
                int tableNumber;
                switch(action){
                    case 1:
                    {
                        iss >> time >> action >> name;
                        handleCome(time, name);
                        break;
                    }
                    case 2:
                    {
                        iss >> time >> action >> name >> tableNumber;
                        handleTakeTable(time, name, tableNumber);
                        break;
                    }
                    case 3:
                    {
                        iss >> time >> action >> name;
                        handleWait(time, name);
                        break;
                    }
                    case 4:
                    {
                        iss >> time >> action >> name;
                        handleLeft(time, name);
                        break;
                    }
                }
            }catch(Error* e){
                std::cout<<e->what()<<std::endl;
            }
        }
        for(const auto& pair : clubMap.clientMap){
            handleKick(clubData.close_time, pair.first);
        }
        for(const auto& pair : clubStat){
            printf("%d %d %d%d:%d%d\n", pair.first, pair.second.totalProfit, (pair.second.timeTaken/60)/10, (pair.second.timeTaken/60)%10, (pair.second.timeTaken%60)/10, (pair.second.timeTaken%60)%10);
        }
        std::cout<<clubData.close_time<<std::endl;
    }else{
        std::cout<<"Не удалось открыть файл.\n";
    }
}

int timeInMinutes(std::string time){
    int hours, minutes;
    std::stringstream ss(time);
    char delimiter;
    ss >> hours >> delimiter >> minutes;
    return hours*60 + minutes;
}