#pragma once

namespace CoordinateUtils {
    template<typename T>
    struct TCoordinates {
        T x;
        T y;
        // ICoordinates(T x, T y) : x(x), y(y) {}

        bool operator==(const TCoordinates<T> &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const TCoordinates<T> &rhs) const {
            return x != rhs.x || y != rhs.y;
        }

        TCoordinates<T> operator+(const TCoordinates<T> &rhs) const {
            return {x + rhs.x, y + rhs.y};
        }

        TCoordinates<T> & operator+=(const TCoordinates<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
    };

    using ICoordinates = TCoordinates<int>;
    using LCoordinates = TCoordinates<long int>;


    bool isAdjacent(const ICoordinates & lhs, const ICoordinates & rhs) {
        for (int x=-1; x < 2; x++) {
            for (int y=-1; y < 2; y++) {
                if (lhs == ICoordinates{rhs.x + x, rhs.y + y}) {
                    return true;
                }
            }
        }
        return false;
    }

    ICoordinates invert(const ICoordinates & rhs) {
        return {-rhs.x, -rhs.y};
    }

};

template<typename T>
using TCoordinates = CoordinateUtils::TCoordinates<T>;
