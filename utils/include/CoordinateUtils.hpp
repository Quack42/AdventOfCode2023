#pragma once

namespace CoordinateUtils {
    struct ICoordinates {
        int x;
        int y;
        // ICoordinates(int x, int y) : x(x), y(y) {}

        bool operator==(const ICoordinates &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const ICoordinates &rhs) const {
            return x != rhs.x || y != rhs.y;
        }

        ICoordinates operator+(const ICoordinates &rhs) const {
            return {x + rhs.x, y + rhs.y};
        }
    };

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
