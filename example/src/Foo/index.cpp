#include "index.h"

void Index::Index(ZeroCouponCurve zeroCouponCurve, double initRate):
        zeroCuoponCurve_{zeroCouponCurve},
        initRate_{initRate},
        frequency_{};

double Index::getRate(int interval) {
    double forwardRate = calculateForward_(interval);
    return convertFromContCompounding(forwardRate); // preguntar si puede ser freq > 1 anyo
}

double Index::calculateForwardRate_(int interval) {
    if (interval <= 1) {
        return initRate;
    }

    const double currentIntervalMaturity = interval * frequency_;
    const double pastIntervalMaturity = (interval - 1) * frequency_;

    const double currentZeroCoupon = ZeroCouponCurve::getZeroCoupon(currentIntervalMaturity);
    const double pastIntervalZeroCoupon = ZeroCouponCurve::getZeroCoupon(pastIntervalMaturity);

    return ((currentZeroCoupon * currentIntervalMaturity)
            - (pastIntervalZeroCoupon * pastIntervalMaturity))
           / (currentIntervalMaturity - pastIntervalMaturity);
}

double Index::convertFromContCompounding_(double forwardRate) {
    int m = 1 / frequency_;
    return m * (exp(forwardRate / m) - 1);
}
