#include <numeric>
#include "swap.h"
#include "fixedLeg.h"

instruments::Swap::Swap(std::unique_ptr<Leg> &payer, std::unique_ptr<Leg> &receiver,
                        std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve)
        : zeroCouponCurve_{zeroCouponCurve} {
    payer_ = std::move(payer);
    receiver_ = std::move(receiver);
}

double instruments::Swap::operator()() const {
    return calculateTotal_(receiver_->getPayments()) - calculateTotal_(payer_->getPayments());
}

// preAnnuity: sum(dfc * discountFactor) of the previous payment dates
double instruments::Swap::computeDiscountFactor(double &preAnnuity) const {
    // Both legs has the same calendar
    std::vector<types::date> paymentCalendar = payer_->getPaymentCalendar();
    types::date from = paymentCalendar.at(paymentCalendar.size() - 2);
    types::date to = payer_->getMaturity();

    // Discount Factor is calculated from fixed leg
    double fixedRate = payer_->isFixed() ? payer_->getRate({}, {}) : receiver_->getRate({}, {});
    double df1 = payer_->isFixed() ? payer_->calculateDayFraction(from, to) : receiver_->calculateDayFraction(from, to);

    double annuity = (1 - (fixedRate * preAnnuity)) /
                     (1 + fixedRate * df1);

    preAnnuity += annuity * df1;
    return annuity;
}


double instruments::Swap::calculateTotal_(const types::payments &payments) const {
    return std::accumulate(payments.begin(),
                           payments.end(),
                           0.0,
                           [&](double acc, const std::pair<types::date, double> &payment) {
                               return acc + calculatePresentValue_(payment.first, payment.second);
                           });
}

double instruments::Swap::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}

