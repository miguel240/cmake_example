#ifndef FIXED_LEG_H
#define FIXED_LEG_H

#include "leg.h"
#include <vector>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "dayCountFraction/dayCountCalculator.h"
#include "util/types.h"

namespace instruments {
    template<class DAY_COUNT_FRACTION>
    class FixedLeg : public Leg {
    public:
        FixedLeg(const std::vector<types::date> &paymentCalendar,
                 double nominal,
                 double rate) : Leg(paymentCalendar, nominal),
                                rate_{rate} {}

        double getPayment(const types::date &from, const types::date &to) const {
            double fractionDate = dcfCalculator_(from, to);
            return nominal_ * rate_ * fractionDate;
        };

        double getRate(types::date from, types::date to) const {
            return rate_;
        }

        void setRate(double rate) {
            rate_ = rate;
        }

        double calculateDayFraction(types::date from, types::date to) const {
            return dcfCalculator_(from, to);
        }

        bool isFixed() const {
            return true;
        }

        ~FixedLeg() {};

    private:
        DAY_COUNT_FRACTION dcfCalculator_;
        double rate_;
    };
}

#endif
