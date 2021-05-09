#ifndef ZERO_COUPON_CURVE_H
#define ZERO_COUPON_CURVE_H

#include "common/types.h"
#include <boost/optional.hpp>

class ZeroCouponCurve {
public:
    ZeroCouponCurve(const types::Map &curveData,
                    types::date today);

    boost::optional<double> getDiscountCurve(types::date date) const;

private:
    types::Map curveData_;
    types::date today_;
};

#endif
