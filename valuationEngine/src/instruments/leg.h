#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>
#include "../util/types.h"
#include "../market/zeroCouponCurve.h"

class Leg {
public:
    Leg(const std::vector<types::date> &paymentCalendar, double nominal)
            : paymentCalendar_{paymentCalendar}, nominal_{nominal} {};

    virtual types::payments getPayments() const {
        types::payments payments{};

        // Obtain the payments (date, payment)
        // calendar contains today's date in the first position
        std::transform(paymentCalendar_.begin(),
                       paymentCalendar_.end() - 1,
                       paymentCalendar_.begin() + 1,
                       std::back_inserter(payments),
                       [&](const types::date &from, const types::date &to) {
                           return std::make_pair(to, getPayment(from, to));
                       });

        return payments;
    }

    virtual double getPayment(types::date from, types::date to) const = 0;

    virtual double calculateDayFraction(types::date from, types::date to) const = 0;

    double getNominal() const {
        return nominal_;
    }

    types::date getMaturity() const {
        return paymentCalendar_.back();
    };

    std::vector<types::date> getPaymentCalendar() const {
        return paymentCalendar_;
    }

    // virtual ~Leg();

protected:
    std::vector<types::date> paymentCalendar_;
    double nominal_;
};

#endif
