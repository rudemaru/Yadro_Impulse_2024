#ifndef _EXCEPTIONS_
#define _EXCEPTIONS_

#include <exception>
#include <string>

class Error : public std::exception {
public:
    Error(const std::string& message) : message_(message) {}
    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif