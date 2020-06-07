#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <iostream>
#include <string>

class Exception
{
public:
    Exception(const std::string &info_) {info = info_;}
    void print_info() {std::cout << info << "\n";}
private:
    std::string info;
};

#endif