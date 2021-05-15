#include <valarray>
#include "zeroCouponCurve.h"
#include "../dayCountFraction/actual_360.h"

market::ZeroCouponCurve::ZeroCouponCurve(types::Map curveData, const types::date today) :
        curveData_{curveData},
        today_{today} {}


boost::optional<double> market::ZeroCouponCurve::getRate(types::date date) const {
    if (isFixedRate) { return fixedRate_; }

    auto zeroCouponElement = getZeroCouponElement_(date);
    return zeroCouponElement ? zeroCouponElement->second : boost::optional<double>();
}

// Return e^-rt or null if not exist the date in the curveData
boost::optional<double> market::ZeroCouponCurve::getDiscountCurve(types::date date) const {
    auto rate = getRate(date);

    // If key doesn't exists
    if (!rate) {
        return boost::optional<double>();
    }

    double fractionDate = dcfCalculator_(today_, date);

    return std::exp(-(*rate) * fractionDate);
}


boost::optional<types::curvePair> market::ZeroCouponCurve::getZeroCouponElement_(const types::date date) const {
    auto it = curveData_.find(date);
    return it == curveData_.end() ? boost::optional<types::curvePair>() : std::make_pair(it->first,
                                                                                         it->second);
};
