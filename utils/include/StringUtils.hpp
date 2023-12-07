#pragma once

#include <string>
#include <vector>

namespace StringUtils {
    std::vector<std::string> split(const std::string & inputString, const std::string & delimiter, bool includeDelimiter = false);
    std::string remove(const std::string & inputString, const std::string & toBeRemovedString);
    bool contains(const std::string & inputString, const std::string & checkIfThisIsInInputString);
    std::string replaceAllCharacters(const std::string & inputString, const char & characterToReplace, const char & characterToReplaceWith);
};
