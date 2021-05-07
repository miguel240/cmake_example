#include <valarray>
#include "zeroCouponCurve.h"
#include "dayCountFraction/actual_360.h"

ZeroCouponCurve::ZeroCouponCurve(const std::map<boost::gregorian::date, double> curveData,
                                 boost::gregorian::date today) : curveData_{curveData}, today_{today} {}

// todo: para que sirve
double ZeroCouponCurve::getZeroCouponRate(boost::gregorian::date date) const {
    return 0;
}

double ZeroCouponCurve::getDiscountCurve(boost::gregorian::date date) const {
    auto it = curveData_.find(date);
    double zeroRate = (it != curveData_.end() ? it->second : -1); // todo: mirar
    double fractionDate = Actual_360::compute_daycount(today_, date);  // todo: mirar

    return std::exp(-zeroRate * fractionDate);
}
