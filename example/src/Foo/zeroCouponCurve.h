#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include <map>

class ZeroCouponCurve {
public:
    ZeroCouponCurve(const std::map<std::string, float> curveData);

    float getZeroCoupon(std::string date);

private:
    const std::map<std::string, float> zeroCurveData_;
};

#endif
