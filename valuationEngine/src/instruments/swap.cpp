#include <numeric>
#include "swap.h"

Swap::Swap(std::unique_ptr<Leg> &payer, std::unique_ptr<Leg> &receiver,
           std::shared_ptr<ZeroCouponCurve> zeroCouponCurve)
        : zeroCouponCurve_{zeroCouponCurve} {
    payer_ = std::move(payer);
    receiver_ = std::move(receiver);
}

double Swap::operator()() const {
    return calculateTotal_(receiver_->getPayments()) - calculateTotal_(payer_->getPayments());
}

double Swap::calculateTotal_(const types::payments &payments) const {
    return std::accumulate(payments.begin(),
                           payments.end(),
                           0.0,
                           [&](double acc, const std::pair<types::date, double> &payment) {
                               return acc + calculatePresentValue_(payment.first, payment.second);
                           });
}

double Swap::calculatePresentValue_(types::date date, double payment) const {
    if (!zeroCouponCurve_->getDiscountCurve(date)) throw "Zero coupon curve incomplete";

    double discountCurve = *zeroCouponCurve_->getDiscountCurve(date);
    return payment * discountCurve;
}
