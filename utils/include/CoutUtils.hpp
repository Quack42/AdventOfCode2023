#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace CoutUtils {
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
    void print(std::string name, T value) {
        std::cout << name << ":[" << value << "]" << std::endl;
    }
};
