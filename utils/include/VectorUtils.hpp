#pragma once

#include <vector>

namespace VectorUtils {

template<typename T>
    std::vector<T> concatenate(const std::vector<T> & lhs, const std::vector<T> & rhs) {
        std::vector<T> ret = lhs;
        ret.insert(ret.end(), rhs.begin(), rhs.end());
        return ret;
    }
};
