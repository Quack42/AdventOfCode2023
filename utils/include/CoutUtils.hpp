#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#define STR(toString) #toString

#define PRINT(variable) CoutUtils::print(std::string(STR(variable)), variable)

namespace CoutUtils {
    template<typename T>
    void print2d(const std::string & name, const std::vector<std::vector<T>> & twoDimensionalArray) {
        if (!name.empty()){
            std::cout << name << std::endl;
        }
        for (const auto & line : twoDimensionalArray) {
            for (const auto & c : line) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }
    template<typename T>
    void print2d(const std::vector<std::vector<T>> & twoDimensionalArray) {
        print2d("", twoDimensionalArray);
    }

    std::string convertToString(const std::string & name, const std::vector<std::string> & input) {
        std::string ret = name + ": [";
        for (unsigned int i=0; i < input.size(); i++) {
            ret += input[i];
            if (i+1 < input.size()) {
                ret += ", ";
            }
        }
        ret += "]";
        return ret;
    }

    template<typename T>
    std::string convertToString(const std::string & name, const std::vector<T> & input) {
        std::string ret = name + ": [";
        for (unsigned int i=0; i < input.size(); i++) {
            ret += std::to_string(input[i]);
            if (i+1 < input.size()) {
                ret += ", ";
            }
        }
        ret += "]";
        return ret;
    }

    template<typename T>
    std::string convertToString(const std::string & name, const std::vector<T> & input, std::function<std::string(const T &)> conversionFunction) {
        std::string ret = name + ": [";
        for (unsigned int i=0; i < input.size(); i++) {
            ret += conversionFunction(input[i]);
            if (i+1 < input.size()) {
                ret += ", ";
            }
        }
        ret += "]";
        return ret;
    }

    template<typename T>
    void print(const std::string & name, const T & value) {
        std::cout << name << ":[" << value << "]" << std::endl;
    }
};
