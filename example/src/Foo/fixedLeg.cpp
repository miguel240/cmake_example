#include "fixedLeg.h"
#include "Foo/fractions/dayCountCalculator.h"


FixedLeg::FixedLeg(const std::vector<std::string> &paymentCalendar,
                   double nominal, double rate) : rate_{rate}, nominal_{nominal} {}


std::vector<std::pair<std::string, double>> FixedLeg::getCalendarWithPayments() const {
    std::vector<std::pair<std::string, double>> payments = {{}};

    std::transform(paymentCalendar_.begin(),
                   paymentCalendar_.end(),
                   std::back_inserter(payments),
                   [&](const std::string &date) { return std::make_pair(date, getPayment(date)); });


    return payments;
}

double FixedLeg::getPayment(std::string date) const {
    return nominal_ * rate_; //* Leg::getDcf(today, date);
}
