#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <cmath>

class MathUtil {
    
    public:
    static float distance(float x0, float y0, float z0, float x1, float y1, float z1) {
        x0 = x0 - x1;
        y0 = y0 - y1;
        z0 = z0 - z1;
        return sqrt(x0 * x0 + y0 * y0 + z0 * z0);
    }
    
};

#endif
