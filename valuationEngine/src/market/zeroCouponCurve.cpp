#include <valarray>
#include "zeroCouponCurve.h"

market::ZeroCouponCurve::ZeroCouponCurve(types::MapDiscountCurveType curveData, const types::date today) :
        curveData_{curveData},
        today_{today},
        fixedRate_{} {}


boost::optional<double> market::ZeroCouponCurve::getRate(types::date date) const {
    if (fixedRate_) return fixedRate_;

    auto it = curveData_.find(date);
    return it != curveData_.end() ? it->second : boost::optional<double>();
}

// Return e^-rt or null if not exist the date in the curveData
boost::optional<double> market::ZeroCouponCurve::getDiscountCurve(types::date date) const {
    auto rate = getRate(date);

    // If key doesn't exists
    if (!rate) return boost::optional<double>();

    double fractionDate = dcfCalculator_(today_, date);
    return std::exp(-(*rate) * fractionDate);
}