#pragma once

#include <string>
#include <vector>

namespace CoutUtils {
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
};
