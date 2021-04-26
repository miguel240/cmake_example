#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include <map>

class ZeroCouponCurve {
public:
    void ZeroCouponCurve();

    double getZeroCoupon(double maturity);

private:
    const map<double, double> zeroCurveData_ = {{0.5, 4.74},
                                                {1,   5},
                                                {1.5, 5.1},
                                                {2,   5.2}};
};

#endif
