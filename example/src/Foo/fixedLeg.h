#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include <vector>
#include <string>
#include "leg.h"

class FixedLeg : public Leg {
public:
    FixedLeg(const std::vector<std::string> &paymentCalendar,
             double nominal, double rate);

    virtual std::vector<std::pair<std::string, double>> getCalendarWithPayments() const;

private:
    double getPayment(std::string date) const;

    // Variables
    const std::vector<std::string> paymentCalendar_;
    const double rate_;
    const double nominal_;
    std::vector<std::pair<std::string, double>> payments_;
};

#endif
