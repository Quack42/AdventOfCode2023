#include "StringUtils.hpp"

std::vector<std::string> StringUtils::split(const std::string & inputString, const std::string & delimiter, bool includeDelimiter) {
    std::vector<std::string> ret;

    auto i = inputString.find(delimiter);
    if (i == std::string::npos) {
        return {inputString};
    } else {
        std::string firstPart = inputString.substr(0, i);
        ret.push_back(firstPart);
    
        std::string unprocessedPart = inputString.substr(i+delimiter.size());

        auto otherParts = StringUtils::split(unprocessedPart, delimiter, includeDelimiter);
        ret.insert(ret.end(), otherParts.begin(), otherParts.end());
    }

    return ret;
}


std::string StringUtils::remove(const std::string & inputString, const std::string & toBeRemovedString) {
    std::string ret;
    auto stringFragments = StringUtils::split(inputString, toBeRemovedString, false);   //this removes the "delimiter"; so use that and just stick it back together afterwards.

    for (auto stringFragment : stringFragments) {
        ret += stringFragment;
    }

    return ret;
}

bool StringUtils::contains(const std::string & inputString, const std::string & checkIfThisIsInInputString) {
    return (inputString.find(checkIfThisIsInInputString) != std::string::npos);
}