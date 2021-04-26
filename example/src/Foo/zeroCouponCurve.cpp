#include "zeroCouponCurve.h"

void ZeroCouponCurve::ZeroCouponCurve() {};

double ZeroCouponCurve::getZeroCoupon(double maturity) {
    auto it = zeroCurveData.find(maturity);
    return it != zeroCurveData.end() ? it->second : null;
}
