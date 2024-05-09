#ifndef _CLUB_DATA_
#define _CLUB_DATA_

#include <iostream>
#include <stdlib.h>

typedef struct
{
    int tables_count;
    std::string open_time;
    std::string close_time;
    int price_per_hour;
}ClubData;

#endif