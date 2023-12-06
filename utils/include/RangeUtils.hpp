#pragma once

#include <algorithm>

namespace RangeUtils {
    template<typename T>
    class Range {
    private:
        T start;     //inclusive
        T end;     //inclusive
    public:
        Range() : start(0), end(0) {}
        Range(T start, T end) : start(start), end(end) {}

        T getStart() const {
            return start;
        }

        T getEnd() const {
            return end;
        }

        T getSize() const {
            if (start > end) {
                return 0;
            }
            return end - start + 1;     //inclusive
        }

        bool empty() const {
            return getSize() == 0;
        }

        bool exceeds(const Range<T> & rhs) const {
            return end > rhs.end;
        }

        bool underceeds(const Range<T> & rhs) const {   //not a real word I think, but whatever
            return start < rhs.start;
        }

        bool inRange(const T & value) const {
            return value >= start && value <= end;
        }
    };


    template<typename T>
    Range<T> intersect(const Range<T> & lhs, const Range<T> & rhs) {
        T overlapStart = std::max(lhs.getStart(), rhs.getStart());
        T overlapEnd = std::min(lhs.getEnd(), rhs.getEnd());
        std::cout << "intersect: lhs" << "[" << lhs.getStart() << ", " << lhs.getEnd() << "]" << std::endl;
        std::cout << "intersect: rhs" << "[" << rhs.getStart() << ", " << rhs.getEnd() << "]" << std::endl;
        std::cout << "intersect: overlapStart" << "[" << overlapStart << "]" << std::endl;
        std::cout << "intersect: overlapEnd" << "[" << overlapEnd << "]" << std::endl;
        if (overlapEnd < overlapStart) {
            return Range<T>(overlapStart, overlapStart-1);
        }
        return Range<T>(overlapStart, std::max(overlapStart, overlapEnd));
    }

    template<typename T>
    bool hasOverlap(const Range<T> & lhs, const Range<T> & rhs) {
        return !intersect(lhs, rhs).empty();
    }

    template<typename T>
    std::vector<Range<T>> split(const Range<T> & lhs, const Range<T> & rhs) {
        Range<T> overlap = intersect(lhs, rhs);
        std::cout << "split: overlap" << "[" << overlap.getStart() << ", " << overlap.getEnd() << "]" << std::endl;
        if (overlap.empty()) {
            return {lhs, rhs};
        }

        std::vector<Range<T>> ret;
        if (lhs.underceeds(rhs)) {
            ret.push_back(Range<T>(lhs.getStart(), std::min(lhs.getEnd(), overlap.getStart())));
        } else if (rhs.underceeds(lhs)) {
            ret.push_back(Range<T>(rhs.getStart(), std::min(rhs.getEnd(), overlap.getStart())));
        }

        ret.push_back(overlap);

        if (lhs.exceeds(rhs)){
            ret.push_back(Range<T>(overlap.getEnd(), std::max(lhs.getEnd(), rhs.getStart())));
        } else if (rhs.exceeds(lhs)){
            ret.push_back(Range<T>(overlap.getEnd(), std::max(rhs.getEnd(), lhs.getStart())));
        }

        return ret;
    }

    template<typename T>
    Range<T> shift(const Range<T> & lhs, const T & rhs) {
        return Range<T>(lhs.getStart() + rhs, lhs.getEnd() + rhs);
    }

    template<typename T>
    bool isSubSet(const Range<T> & lhs, const Range<T> & rhs) {
        return rhs.inRange(lhs.getStart()) && rhs.inRange(lhs.getEnd());
    }



};
