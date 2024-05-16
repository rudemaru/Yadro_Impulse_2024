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
    if(clubMap.currentClients.contains(clientName)){
        throw new Error(comeTime + " 13 YouShallNotPass");
        return;
    }

   TableData emptyTable = {0, ""};

   clubMap.currentClients.insert(clientName);
   clubMap.clientMap.insert(std::make_pair(clientName, emptyTable));
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

bool ActionHandler::isFree(int tableNumber){
    auto it_ft = std::find(clubMap.freeTables.begin(), clubMap.freeTables.end(), tableNumber);
    if(it_ft != clubMap.freeTables.end()){
        return true;
    }
    return false;
}

void ActionHandler::handleTakeTable(const std::string comeTime, const std::string clientName, int tableNumber) {
    if(clubMap.currentClients.contains(clientName)){
        if(isFree(tableNumber)){
            if(clubMap.clientMap[clientName].number != 0){
                TableData current_table = clubMap.clientMap[clientName];

                int session_time = timeInMinutes(comeTime) - timeInMinutes(current_table.last_start);
                clubStat[current_table.number].timeTaken += session_time;
                clubStat[current_table.number].totalProfit += ((session_time)/60)*clubData.price_per_hour;
                if(session_time - (session_time/60)*60 != 0){
                    clubStat[current_table.number].totalProfit += clubData.price_per_hour;
                }
                clubMap.freeTables.push_back(current_table.number);
            }

            clubMap.clientMap[clientName].number = tableNumber;
            clubMap.clientMap[clientName].last_start = comeTime;

            auto it_ft = std::find(clubMap.freeTables.begin(), clubMap.freeTables.end(), tableNumber);
            if(it_ft != clubMap.freeTables.end()){
                clubMap.freeTables.erase(it_ft);
            }

            auto it_cq = std::find(clubMap.clientQueue.begin(), clubMap.clientQueue.end(), clientName);
            if(it_cq != clubMap.clientQueue.end()){
                clubMap.clientQueue.erase(it_cq);
            }

        }else{
            throw new Error(comeTime + " 13 PlaceIsBusy");
        }
    }else{
        throw new Error(comeTime + " 13 ClientUnknown");
    }
}

void ActionHandler::handleLeft(const std::string comeTime, const std::string clientName){
    if(clubMap.currentClients.contains(clientName)){
        TableData current_table = clubMap.clientMap[clientName];
        if(current_table.number != 0){
            int session_time = timeInMinutes(comeTime) - timeInMinutes(current_table.last_start);
            clubStat[current_table.number].timeTaken += session_time;
            clubStat[current_table.number].totalProfit += ((session_time)/60)*clubData.price_per_hour;
            if(session_time - (session_time/60)*60 != 0){
                clubStat[current_table.number].totalProfit += clubData.price_per_hour;
            }
            clubMap.freeTables.push_back(current_table.number);
        }
        clubMap.clientMap.erase(clientName);
        clubMap.currentClients.erase(clientName);

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
        int freeTable = clubMap.freeTables.back();
        
        TableData newTable = {freeTable, comeTime};

        clubMap.clientMap[clientName] = newTable;

        std::cout<<comeTime<<" 12 "<<clientName<<" "<<clubMap.clientMap[clientName].number<<std::endl;

        clubMap.clientQueue.erase(clubMap.clientQueue.begin());

        clubMap.freeTables.pop_back();
    }else{
        return;
    }
}

void ActionHandler::handleKick(const std::string comeTime, const std::string clientName) {
    
    TableData current_table = clubMap.clientMap[clientName];

    if(current_table.number != 0){
        int session_time = timeInMinutes(comeTime) - timeInMinutes(current_table.last_start);
        clubStat[current_table.number].timeTaken += session_time;
        clubStat[current_table.number].totalProfit += ((session_time)/60)*clubData.price_per_hour;
        if(session_time - (session_time/60)*60 != 0){
            clubStat[current_table.number].totalProfit += clubData.price_per_hour;
        }
    }
    
    auto it = std::find(clubMap.clientQueue.begin(), clubMap.clientQueue.end(), clientName);
        if(it != clubMap.clientQueue.end()){
            clubMap.clientQueue.erase(it);
        }
    
    clubMap.currentClients.erase(clientName);
    clubMap.clientMap.erase(clientName);

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
            TableStat cs = {0,0};
            clubStat.insert(std::make_pair(i, cs));
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
        std::map<std::string, TableData> clientMap_trace = clubMap.clientMap; 
        for (auto pair : clientMap_trace) {
            handleKick(clubData.close_time, pair.first);
        }
        std::cout<<clubData.close_time<<std::endl;
        for (std::map<int, TableStat>::iterator it = clubStat.begin(); it!= clubStat.end(); ++it) {
            printf("%d %d %d%d:%d%d\n", it->first, it->second.totalProfit, (it->second.timeTaken / 60) / 10, (it->second.timeTaken / 60) % 10, (it->second.timeTaken % 60) / 10, (it->second.timeTaken % 60) % 10);
        }
    }else{
        std::cout<<"Ошибка при открытии файла.\n";
    }
}

int timeInMinutes(std::string time){
    int hours, minutes;
    std::stringstream ss(time);
    char delimiter;
    ss >> hours >> delimiter >> minutes;
    return hours*60 + minutes;
}