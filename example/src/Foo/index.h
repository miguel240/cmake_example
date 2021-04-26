#ifndef INDEX_H
#define INDEX_H

#include <ZeroCouponCurve>

class Index {
public:
    Index(ZeroCouponCurve zeroCouponCurve);

    double getRate(double year);

private:
    double calculateForwardRate_(int interval); // buscar convencion
    double convertFromContCompounding_(double forwardRate);

    ZeroCouponCurve zeroCuoponCurve_;
    double initRate_;
    double frequency_;
};

#endif
