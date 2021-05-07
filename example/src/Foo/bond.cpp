
#include "bond.h"


Bond::Bond(std::unique_ptr<Leg> &fixedLeg, std::shared_ptr<ZeroCouponCurve> &zeroCouponCurve) :
        zeroCouponCurve_{zeroCouponCurve} {
    fixedLeg_ = std::move(fixedLeg);
}
// cambiar para que devuelva el nominal
double Bond::operator()() const {
    auto paymentCalendar = fixedLeg_->getCalendarWithPayments();
    double price = 0.0;
    for (auto it : paymentCalendar) {
        price += it.second * zeroCouponCurve_->getDiscountCurve(it.first);
    } // todo: accumulate

    return price;
}

