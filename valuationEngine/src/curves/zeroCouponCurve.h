#ifndef ZERO_COUPON_CURVE_H
#define ZERO_COUPON_CURVE_H

#include "../util/types.h"
#include <boost/optional/optional_io.hpp>

class ZeroCouponCurve {
public:
    ZeroCouponCurve(types::Map curveData, types::date today);

    boost::optional<double> getDiscountCurve(types::date date) const;

private:
    types::Map curveData_;
    types::date today_;
};

#endif
