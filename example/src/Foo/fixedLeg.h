#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include "leg.h"
#include <vector>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>

class FixedLeg : public Leg {
public:
    FixedLeg(const std::vector<boost::gregorian::date> &paymentCalendar,
             double nominal, double rate);

    virtual std::vector<std::pair<boost::gregorian::date, double>> getCalendarWithPayments() const;

private:
    double getPayment(boost::gregorian::date date) const;

    // Variables
    const std::vector<boost::gregorian::date> paymentCalendar_;
    const double rate_;
    const double nominal_;
    std::vector<std::pair<boost::gregorian::date, double>> payments_;
};

#endif
