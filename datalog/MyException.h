#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include <exception>
#include <iostream>

class MyException : public std::exception {
private:
    std::string message;
public:
    MyException(const std::string& message) noexcept : message(message) {
        //std::cout << this->message << std::endl; //uncomment in order to see exceptions without catching
    }
    const char* what() const noexcept override {
        return this->message.c_str();
    }
};

#endif