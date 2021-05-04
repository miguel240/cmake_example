#include "fixedLeg.h"
#include "Foo/fractions/dayCountCalculator.h"


FixedLeg::FixedLeg(std::vector<std::string> paymentCalendar, float interest, float nominal) :
        paymentCalendar_{paymentCalendar},
        interest_{interest},
        nominal_{nominal},
        payments_{} {};

float FixedLeg::getPrice() {
    for (const auto date: paymentCalendar_) {
        //float price = nominal_ * interest_ * leg::getDcf(today, date);
        // payments_.push_back(std::make_pair(date, price));
    }
    // leg::getCurrentPrice(payments_); // Aplica el factor descuento
    return 1;
}
