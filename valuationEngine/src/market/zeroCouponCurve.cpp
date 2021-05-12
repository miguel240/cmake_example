#include <valarray>
#include "zeroCouponCurve.h"
#include "../dayCountFraction/actual_360.h"

ZeroCouponCurve::ZeroCouponCurve(types::Map curveData, const types::date today) :
        curveData_{curveData},
        today_{today} {}


boost::optional<double> ZeroCouponCurve::getRate(types::date date) const {
    auto zeroCouponElement = getZeroCouponElement_(date);
    return zeroCouponElement ? zeroCouponElement->second : boost::optional<double>();
}

// Return e^-rt or null if not exist the date in the curveData
boost::optional<double> ZeroCouponCurve::getDiscountCurve(types::date date) const {
    auto zeroCouponElement = getZeroCouponElement_(date);

    // If key doesn't exists
    if (!zeroCouponElement) {
        return boost::optional<double>();
    }

    double rate = zeroCouponElement->second;
    double fractionDate = dcfCalculator_(today_, date);

    return std::exp(-rate * fractionDate);
}


boost::optional<types::curvePair> ZeroCouponCurve::getZeroCouponElement_(const types::date date) const {
    auto it = curveData_.find(date);
    return it == curveData_.end() ? boost::optional<types::curvePair>() : std::make_pair(it->first,
                                                                                         it->second);
};
