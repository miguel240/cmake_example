#include <valarray>
#include "zeroCouponCurve.h"
#include "../dayCountFraction/actual_360.h"

ZeroCouponCurve::ZeroCouponCurve(types::Map curveData, const types::date today) :
        curveData_{curveData},
        today_{today} {}


// Return e^-rt or null if not exist the date in the curveData
boost::optional<double> ZeroCouponCurve::getDiscountCurve(types::date date) const {
    auto it = curveData_.find(date);

    // If key doesn't exists
    if (it == curveData_.end()) {
        return boost::optional<double>();
    }
    double rate = it->second;
    double fractionDate = Actual_360()(today_, date);

    return std::exp(-rate * fractionDate);
}

