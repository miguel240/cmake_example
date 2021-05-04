#include "zeroCouponCurve.h"

ZeroCouponCurve::ZeroCouponCurve(const std::map<std::string, float> curveData) :
        zeroCurveData_{curveData} {};

float ZeroCouponCurve::getZeroCoupon(const std::string date) {
    auto it = zeroCurveData_.find(date);
    return it != zeroCurveData_.end() ? it->second : -1; // como devuelvo null ?
}
