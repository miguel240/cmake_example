#ifndef ZERO_COUPON_CURVE_H
#define ZERO_COUPON_CURVE_H

#include <map>
#include "common/types.h"
#include <boost/optional.hpp>

class ZeroCouponCurve {
public:
    ZeroCouponCurve(const std::map<types::date, double> &curveData,
                    types::date today);

    boost::optional<double> getDiscountCurve(types::date date) const;

private:
    std::map<types::date, double> curveData_;
    types::date today_;
};

#endif
