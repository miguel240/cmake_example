#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include "leg.h"
#include <vector>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "dayCountFraction/dayCountCalculator.h"
#include "util/types.h"

template<class DAY_COUNT_FRACTION>
class FixedLeg : public Leg {
public:
    FixedLeg(const std::vector<types::date> &paymentCalendar,
             double nominal,
             double rate) : Leg(paymentCalendar),
                            paymentCalendar_{paymentCalendar},
                            rate_{rate},
                            nominal_{nominal} {
        // calendar contains today's date in the first position
        today_ = paymentCalendar.at(0);
    }

    virtual types::payments getPayments() const {
        types::payments payments{};

        for (auto it = paymentCalendar_.begin(); it != paymentCalendar_.end() - 1; ++it) {
            auto newPayment = std::make_pair(*(it + 1), getPayment_(*it, *(it + 1))); //todo
            payments.push_back(newPayment);
        }

        /*std::transform(paymentCalendar_.begin(),
                       paymentCalendar_.end() - 1,
                       std::back_inserter(payments),
                       [&](const types::date &from, const types::date &to) {
                           return std::make_pair(from, getPayment(from, to));
                       });*/

        return payments;
    }

    virtual double calculateDayFraction(types::date from, types::date to) const {
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
    double getPayment_(types::date from, types::date to) const {
        double fractionDate = dcfCalculator_(from, to);
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
