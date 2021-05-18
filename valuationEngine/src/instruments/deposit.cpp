#include "deposit.h"

instruments::Deposit::Deposit(std::unique_ptr<Leg> &fixedLeg, std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve)
        : zeroCouponCurve_{zeroCouponCurve} {
    fixedLeg_ = std::move(fixedLeg);
}


double instruments::Deposit::operator()() const {
    types::payments payments = fixedLeg_->getPayments();
    if (payments.size() == 0) return 0;

    std::pair<types::date, double> payment = payments.at(0);
    return calculatePresentValue_(payment.first, payment.second + fixedLeg_->getNominal());
}

// preAnnuity: sum(dfc * discountFactor) of the previous payment dates
double instruments::Deposit::computeDiscountFactor(double &preAnnuity) const {
    std::vector<types::date> paymentCalendar = fixedLeg_->getPaymentCalendar();
    types::date from = paymentCalendar.at(0); // today
    types::date to = fixedLeg_->getMaturity();

    double annuity = 1 / (1 + fixedLeg_->getRate({}, {}) * fixedLeg_->calculateDayFraction(from, to));
    preAnnuity = annuity * fixedLeg_->calculateDayFraction(from, to);

    return annuity;
}


double instruments::Deposit::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}