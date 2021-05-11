#ifndef FLOATING_LEG_H
#define FLOATING_LEG_H

#include <utility>
#include <string>
#include <vector>

class FloatingLeg {
public:
    FloatingLeg(const std::vector <std::string> &paymentCalendar,
                double nominal, double index);

    virtual std::vector<std::pair<std::string, double>> getCalendarWithPayments() const;

private:
    double getPayment(std::string date) const;

    // Variables
    const std::vector<std::string> paymentCalendar_;
    const double nominal_;
    const double index_;
    std::vector<std::pair<std::string, double>> payments_;
};

#endif
