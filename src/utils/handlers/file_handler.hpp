#ifndef _FILE_HANDLER_  
#define _FILE_HANDLER_

#include "club_data.hpp"
#include <stdlib.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

class FileHandler {
    public:
        static void validCheck(ClubData &cd, std::string fp); 
};

#endif 