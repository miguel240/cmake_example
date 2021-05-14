#include "index.h"

market::Index::Index(float indexFrequency, std::shared_ptr<ZeroCouponCurve> zeroCouponCurve) :
        indexFrequency_{indexFrequency},
        zeroCouponCurve_{zeroCouponCurve},
        today_{zeroCouponCurve->getTodayDate()} {}

double market::Index::calculateForwardRate(const types::date &from, const types::date &to) const {
    double currentYearFraction = dcfCalculator_(today_, to);
    double previousYearFraction = dcfCalculator_(today_, from);

    if (!zeroCouponCurve_->getRate(to) || !zeroCouponCurve_->getRate(from)) {
        throw "Zero coupon curve incomplete";
    }

    double forwardRate = ((*zeroCouponCurve_->getRate(to) * currentYearFraction) -
                          (*zeroCouponCurve_->getRate(from) * previousYearFraction)) /
                         (currentYearFraction - previousYearFraction);

    return getAnnualizedRate_(forwardRate);
}

double market::Index::getAnnualizedRate_(double rate) const {
    return indexFrequency_ * (std::exp(rate / indexFrequency_) - 1);
}
