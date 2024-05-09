#include "action_handler.hpp"

void ActionHandler::handleCome(std::string comeTime, std::string clientName) {
    if(comeTime < clubData.open_time || comeTime >= clubData.close_time){
        throw new Error("NotOpenYet");
        return;
    }
    if(clubMap.clientMap.contains(clientName)){
        throw new Error("YouShallNotPass");
        return;
    }
    ClientData clientData = {comeTime, "", clientName, 0};
    clubMap.clientQueue.push(clientData);
}