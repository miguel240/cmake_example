#include "bond.h"
#include <numeric>
#include <stdexcept>

instruments::Bond::Bond(std::unique_ptr<Leg> &fixedLeg, std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve) :
        zeroCouponCurve_{zeroCouponCurve} {
    fixedLeg_ = std::move(fixedLeg);
}

double instruments::Bond::operator()() const {
    types::payments payments = fixedLeg_->getPayments();

    // In a bond the nominal is returned at maturity
    // Fixed leg doesn't contemplate this
    payments.push_back(std::make_pair(fixedLeg_->getMaturity(),
                                      fixedLeg_->getNominal()));

    return std::accumulate(payments.begin(),
                           payments.end(),
                           0.0,
                           [&](double acc, std::pair<types::date, double> &payment) {
                               return acc + calculatePresentValue_(payment.first, payment.second);
                           });
}

// preAnnuity: sum(dfc * discountFactor) of the previous payment dates
double instruments::Bond::computeDiscountFactor(double &preAnnuity) const {
    std::vector<types::date> paymentCalendar = fixedLeg_->getPaymentCalendar();
    types::date from = paymentCalendar.at(paymentCalendar.size() - 2);
    types::date to = fixedLeg_->getMaturity();

    double fixedRate = fixedLeg_->getRate({}, {});
    auto annuity = (1 - (fixedRate * preAnnuity)) /
                   (1 + fixedRate * fixedLeg_->calculateDayFraction(from, to));

    preAnnuity += annuity * fixedLeg_->calculateDayFraction(from, to);
    return annuity;
};

double instruments::Bond::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}

