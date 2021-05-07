#include <algorithm>
#include <dayCountFraction/actual_360.h>
#include "fixedLeg.h"
#include "bond.h"


FixedLeg::FixedLeg(const std::vector<types::date> &paymentCalendar,
                   double nominal,
                   double rate) : rate_{rate}, nominal_{nominal} {
    // calendar contains today's date in the first position
    today_ = paymentCalendar.at(0);
}

types::payments FixedLeg::getPayments() const {
    types::payments payments = {{}};

    std::transform(paymentCalendar_.begin(),
                   paymentCalendar_.end(),
                   std::back_inserter(payments),
                   [&](const types::date &date) { return std::make_pair(date, getPayment(date)); });

    return payments;
}

double FixedLeg::getPayment(types::date date) const {
    double fractionDate = Actual_360::compute_daycount(today_, date);  // todo: mirar
    return nominal_ * rate_ * fractionDate;
}
