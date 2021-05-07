#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include "leg.h"
#include <vector>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "common/types.h"

class FixedLeg : public Leg {
public:
    FixedLeg(const std::vector<types::date> &paymentCalendar,
             double nominal,
             double rate);

    virtual types::payments getPayments() const;

    virtual types::date getMaturity() const {
        return paymentCalendar_.back();
    }

    virtual double getNominal() const {
        return nominal_;
    }

private:
    double getPayment(boost::gregorian::date date) const;

    // Variables
    std::vector<types::date> paymentCalendar_;
    double rate_;
    double nominal_;
    types::date today_;
};

#endif
