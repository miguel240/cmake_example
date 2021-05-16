#include "deposit.h"

instruments::Deposit::Deposit(std::unique_ptr<Leg> &fixedLeg, std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve)
        : zeroCouponCurve_{zeroCouponCurve} {
    fixedLeg_ = std::move(fixedLeg);
}


double instruments::Deposit::operator()() const {
    types::payments paymentCalendar = fixedLeg_->getPayments();
    if (paymentCalendar.size() == 0) return 0;

    std::pair<types::date, double> payment = paymentCalendar.at(0);
    return calculatePresentValue_(payment.first, payment.second + fixedLeg_->getNominal());
}

double instruments::Deposit::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}

