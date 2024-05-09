#include "file_handler.hpp"

FileHandler::FileHandler(std::string fp){
    this->file_path = fp;
}

void FileHandler::validCheck(ClubData cd){
    std::ifstream file(file_path);
    if(file.is_open()){
        std::string temp_str;

        std::regex int_regex(R"(\d+$)");
        std::regex time_regex(R"(([01]?[0-9]|2[0-3]):([0-5]\d) ([01]?[0-9]|2[0-3]):([0-5]\d)$)");
        std::regex name_regex(R"([a-z0-9_-]+)");
        std::regex event_regex(R"((?:0[0-9]|1[0-9]|2[0-3]):[0-5][0-9] ([1-4]) ([a-z0-9_-]+(?: \d+)?))");

        std::getline(file, temp_str);
        if(std::regex_match(temp_str, int_regex)){
            cd.tables_count = std::stoi(temp_str);
        }else{
            std::cout<<temp_str;
            exit(0);
        }

        std::getline(file, temp_str);
        if(std::regex_match(temp_str, time_regex)){
            cd.open_time = temp_str.substr(0, temp_str.find(' '));
            cd.close_time = temp_str.substr(temp_str.find(' ') + 1, temp_str.size() - 1);
        }else{
            std::cout<<temp_str;
            exit(0);
        }

        std::getline(file, temp_str);
        if(std::regex_match(temp_str, int_regex)){
            cd.price_per_hour = std::stoi(temp_str);
        }else{
            std::cout<<temp_str;
            exit(0);
        }

        std::cout<<"tables_count: "<<cd.tables_count<<std::endl;
        std::cout<<"open_time: "<<cd.open_time<<"_\n";
        std::cout<<"close_time: "<<cd.close_time<<"_\n";
        std::cout<<"price_per_hour: "<<cd.price_per_hour<<std::endl;

        while(std::getline(file, temp_str)){
            if(std::regex_match(temp_str, event_regex)){
                std::cout<<temp_str<<std::endl;
            }else{
                std::cout<<temp_str;
                exit(0);
            }
        }
    }else{
        std::cout<<"Не удалось открыть файл\n";
    }
}