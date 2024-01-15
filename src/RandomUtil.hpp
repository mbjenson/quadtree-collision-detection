#include <random>

#ifndef RAND_UTIL_H
#define RAND_UTIL_H

inline float getRandVelocity(int absDeadZone, int absRange) {
    float val = 0.f;
    if (absRange > absDeadZone) {
        val = rand() % (absRange - absDeadZone) + absDeadZone;
        int sign = rand() % 2 - 1;
        if (sign < 0)
            val = -val;
    }
    return val;
}

inline float getRandInRange(int minVal, int maxVal) {
    float val = 0.f;
    if (maxVal > minVal) {
        if (maxVal != 0) {
            if (minVal == maxVal) {
                val = rand() % maxVal;
            }
            else {
                val = rand() % (maxVal - minVal) + minVal;
            }
        }
    }
    if (maxVal == minVal) {
        val = maxVal;
    }
    return val;
}

#endif