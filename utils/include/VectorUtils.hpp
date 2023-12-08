#pragma once

#include <vector>
#include <queue>

namespace VectorUtils {

    template<typename T>
    std::vector<T> concatenate(const std::vector<T> & lhs, const std::vector<T> & rhs) {
        std::vector<T> ret = lhs;
        ret.insert(ret.end(), rhs.begin(), rhs.end());
        return ret;
    }

    template<typename T>
    bool contains(const std::vector<T> & lhs, const T & rhs) {
        return std::find(lhs.begin(), lhs.end(), rhs) != lhs.end();
    }

    template<typename T>
    std::vector<T> remove(const std::vector<T> & lhs, const std::vector<T> & rhs) {
        std::vector<T> ret;
        for (const auto & t : lhs) {
            if (!contains(rhs, t)) {
                ret.push_back(t);
            }
        }
        return ret;
    }

    template<typename T>
    std::vector<T> intersection(std::vector<T> lhs, std::vector<T> rhs) {
        std::vector<T> ret;

        std::sort(lhs.begin(), lhs.end());
        std::sort(rhs.begin(), rhs.end());

        std::set_intersection(lhs.begin(),lhs.end(),
                              rhs.begin(),rhs.end(),
                              back_inserter(ret));
        return ret;
    }

    template<typename T>
    std::queue<T> convertToQueue(const std::vector<T> & toQueue) {
        std::queue<T> ret;
        for (const auto & t : toQueue) {
            ret.push(t);
        }
        return ret;
    }
};
