#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include "leg.h"
#include <vector>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "dayCountFraction/dayCountCalculator.h"
#include "common/types.h"

template<class DAY_COUNT_FRACTION>
class FixedLeg : public Leg {
public:
    FixedLeg(const std::vector<types::date> &paymentCalendar,
             double nominal,
             double rate) : Leg(paymentCalendar), rate_{rate}, nominal_{nominal} {
        // calendar contains today's date in the first position
        today_ = paymentCalendar.at(0);
    }

    virtual types::payments getPayments() const {
        types::payments payments = {{}};

        std::transform(paymentCalendar_.begin() + 1, // todo: posible overflow
                       paymentCalendar_.end(),
                       std::back_inserter(payments),
                       [&](const types::date &date) { return std::make_pair(date, getPayment(date)); });

        return payments;
    }

    virtual double calculateDayFraction(types::date from, types::date to) const{
        return dcfCalculator_(from, to);
    }

    virtual types::date getMaturity() const {
        return paymentCalendar_.back();
    }

    virtual double getRate() const {
        return rate_;
    }

    virtual double getNominal() const {
        return nominal_;
    }

private:
    double getPayment(boost::gregorian::date date) const {
        double fractionDate = dcfCalculator_(today_, date);
        return nominal_ * rate_ * fractionDate;
    };

    // Variables
    DAY_COUNT_FRACTION dcfCalculator_;
    std::vector<types::date> paymentCalendar_;
    double rate_;
    double nominal_;
    types::date today_;
};

#endif
