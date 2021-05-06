#include "floatingLeg.h"

FloatingLeg::FloatingLeg(const std::vector <std::string> &paymentCalendar, double nominal, double index) :
        nominal_{nominal},
        index_{index} {}

std::vector<std::pair<std::string, double>> FloatingLeg::getCalendarWithPayments() const {
    std::vector<std::pair<std::string, double>> payments = {{}};

    //todo: calculate payments
    return payments;
}

double FloatingLeg::getPayment(std::string date) const {
    //todo: calculate payment
    return 0.0;
}
