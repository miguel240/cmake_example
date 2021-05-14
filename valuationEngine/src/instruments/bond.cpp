#include "bond.h"
#include <numeric>
#include <stdexcept>

instruments::Bond::Bond(std::unique_ptr<Leg> &fixedLeg, std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve) :
        zeroCouponCurve_{zeroCouponCurve} {
    fixedLeg_ = std::move(fixedLeg);
}

double instruments::Bond::operator()() const {
    types::payments paymentCalendar = fixedLeg_->getPayments();

    // In a bond the nominal is returned at maturity
    // Fixed leg doesn't contemplate this
    paymentCalendar.push_back(std::make_pair(fixedLeg_->getMaturity(),
                                             fixedLeg_->getNominal()));

    return std::accumulate(paymentCalendar.begin(),
                           paymentCalendar.end(),
                           0.0,
                           [&](double acc, std::pair<types::date, double> &payment) {
                               return acc + calculatePresentValue_(payment.first, payment.second);
                           });
}

double instruments::Bond::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}

