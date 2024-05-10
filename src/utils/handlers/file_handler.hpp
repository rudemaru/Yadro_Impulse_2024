#ifndef _FILE_HANDLER_  
#define _FILE_HANDLER_

#include <stdlib.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>
#include "../structs/club_data.hpp"

class FileHandler {
    public:
        static void validCheck(ClubData &cd, std::string fp); 
};

#endif 