#include <valarray>
#include "zeroCouponCurve.h"
#include "dayCountFraction/actual_360.h"

ZeroCouponCurve::ZeroCouponCurve(const types::Map &curveData,
                                 types::date today) : curveData_{curveData}, today_{today} {}


// Return e^-rt or null if not exist the date in the curveData
boost::optional<double> ZeroCouponCurve::getDiscountCurve(types::date date) const {
    auto it = curveData_.find(date);

    // Key doesn't exists
    if (it == curveData_.end()) {
        return boost::optional<double>();
    }

    double rate = it->second;
    double fractionDate = Actual_360::computeDaycount(today_, date);  // todo: mirar, preguntar

    return std::exp(-rate * fractionDate);
}
