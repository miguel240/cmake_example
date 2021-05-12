#ifndef INDEX_H
#define INDEX_H

#include <memory>
#include <util/types.h>
#include <dayCountFraction/actual_360.h>
#include "zeroCouponCurve.h"

class Index {
public:
    // indexFrequency: times in a year (Euribor 6M = 2)
    Index(float indexFrequency, std::shared_ptr<ZeroCouponCurve> zeroCouponCurve);

    double calculateForwardRate(const types::date &from, const types::date &to) const;

private:
    double getAnnualizedRate_(double rate) const;

    // Variables
    Actual_360 dcfCalculator_;
    float indexFrequency_;
    types::date today_;
    std::shared_ptr<ZeroCouponCurve> zeroCouponCurve_;
};


#endif