#pragma once

#include <vector>
#include <queue>

namespace VectorUtils {

    bool convertToMap(const std::vector<std::string> & lines, std::vector<std::vector<char> > & map) {
        if (lines.empty()) {
            return true;
        }
        bool equalLines = true;
        const std::string & firstLine = lines[0];
        for (const std::string & line : lines) {
            std::vector<char> mapLine;
            if (line.size() != firstLine.size()) {
                equalLines = false;
            }
            for (const char & c : line) {
                mapLine.push_back(c);
            }
            map.push_back(mapLine);
        }

        return equalLines;
    }

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
