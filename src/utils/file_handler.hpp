#ifndef _FILE_HANDLER_  
#define _FILE_HANDLER_

#include "club_data.hpp"
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

class FileHandler {

    std::string file_path;

    public:
        FileHandler(std::string);
        /*Проверяет файл на соответствие шаблону
          входных данных и заполняет структуру
          информации о клубе*/
        void validCheck(ClubData); 
};

#endif 