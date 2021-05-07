#include <algorithm>
#include <fractions/actual_360.h>
#include "fixedLeg.h"


FixedLeg::FixedLeg(const std::vector<boost::gregorian::date> &paymentCalendar,
                   double nominal, double rate) : rate_{rate}, nominal_{nominal} {}


std::vector<std::pair<boost::gregorian::date, double>> FixedLeg::getCalendarWithPayments() const {
    std::vector<std::pair<boost::gregorian::date, double>> payments = {{}};

    std::transform(paymentCalendar_.begin(),
                   paymentCalendar_.end(),
                   std::back_inserter(payments),
                   [&](const boost::gregorian::date &date) { return std::make_pair(date, getPayment(date)); });


    return payments;
}

double FixedLeg::getPayment(boost::gregorian::date date) const {
    double fractionDate = Actual_360::compute_daycount(today_, date);  // todo: mirar
    return nominal_ * rate_; //* Leg::getDcf(today, date);
}
