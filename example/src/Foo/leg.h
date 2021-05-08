#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "common/types.h"

class Leg {
public:
    Leg(const std::vector<types::date> &paymentCalendar)
            : paymenCalendar_{paymentCalendar} {};

    virtual types::payments getPayments() const = 0;

    virtual double calculateDayFraction(types::date from, types::date to) const = 0;

    virtual double getNominal() const = 0;

    virtual double getRate() const = 0;

    virtual types::date getMaturity() const = 0;

    std::vector<types::date> getPaymentCalendar() const {
        return paymenCalendar_;
    }

    virtual ~Leg();

private:
    std::vector<types::date> paymenCalendar_;
};

#endif