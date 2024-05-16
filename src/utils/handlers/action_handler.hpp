#ifndef _ACTION_HANDLER_
#define _ACTION_HANDLER_

#include "../structs/club_data.hpp"
#include "../structs/table_stat.hpp"
#include "../structs/club_map.hpp"
#include "../exceptions/exceptions.hpp"
#include <fstream>
#include <sstream>
#include <regex>

class ActionHandler{

    ClubData clubData;

    std::map<int, TableStat> clubStat;

    ClubMap clubMap;

    public:
        ActionHandler(ClubData cb, ClubMap cm);
        /*
            ID 1. Клиент пришел
            Формат: время имя клиента
            Если клиент уже в компьютерном клубе, генерируется ошибка YouShallNotPass
            Если клиент пришел в нерабочие часы, тогда NotOpenYet
        */
        void handleCome(const std::string comeTime, const std::string clientName);
        /*
            ID 2. Клиент сел за стол
            Формат: время имя клиента номер стола
            Если клиент уже сидит за столом, то он может сменить стол.
            Если стол "номер стола" занят (в том числе, если клиент пытается пересесть за
            стол, за которым сам и сидит), генерируется ошибка PlaceIsBusy.
            Если клиент не находится в компьютерном клубе, генерируется ошибка ClientUnknown.
        */
        void handleTakeTable(const std::string comeTime, const std::string clientName, int tableNumber);
        /*
            ID 3. Клиент ожидает
            Формат: время имя клиента
            Если в клубе есть свободные столы, то генерируется ошибка ICanWaitNoLonger!
            Если в очереди ожидания клиентов больше, чем общее число столов, то клиент
            уходит и генерируется событие ID 11.
        */
        void handleWait(const std::string comeTime, const std::string clientName);
        /*
            ID 4. Клиент ушел
            Формат: время имя клиента
            Если клиент не находится в компьютерном клубе, 
            генерируется ошибка ClientUnknown.
            Когда клиент уходит, стол, за которым он сидел освобождается и его занимает
            первый клиент из очереди ожидания (ID 12).
        */
        void handleLeft(const std::string comeTime, const std::string clientName);
        /*
            ID 11. Клиент ушел
            Формат: время имя клиента
            Генерируется в конце рабочего дня для всех клиентов, оставшихся в компьютерном
            клубе, в алфавитном порядке их имен. А также, когда клиент встает в очередь, а
            очередь ожидания уже заполнена.
        */
        void handleKick(const std::string comeTime, const std::string clientName);
        /*
            ID 12. Клиент сел за стол
            Формат: время имя клиента номер стола
            Генерируется для первого клиента в очереди при освобождении любого стола.
        */
        void handleTakeFree(const std::string comeTime);
        /*
            ID 13. Ошибка
            Формат: время ошибка
            Выводится сразу после события, которое вызвало ошибку. Событие, вызвавшее
            ошибку, считается не выполненным, и никакого эффекта на состояние клиентов не
            оказывает.
        */

        void run(ClubData cd, std::string fp);

        bool isFree(int);

};

int timeInMinutes(std::string time);

#endif