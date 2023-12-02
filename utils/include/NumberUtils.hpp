#pragma once

#include <algorithm>

namespace NumberUtils {
    template <typename T, typename T2>
    bool getHighestElement(const T & listOfNumbers, T2 & out) {
        if (listOfNumbers.empty()) {
            return false;
        }
        out = listOfNumbers[0];

        for (const T2 & listValue : listOfNumbers) {
            out = std::max(out, listValue);
        }

        return true;
    }
};
