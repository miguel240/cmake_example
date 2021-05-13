#ifndef FLOATING_LEG_H
#define FLOATING_LEG_H

#include "dayCountFraction/dayCountCalculator.h"
#include "leg.h"
#include "../market/index.h"

template<class DAY_COUNT_FRACTION>
class FloatingLeg : public Leg {
public:
    FloatingLeg(const std::vector<types::date> &paymentCalendar,
                double nominal,
                std::shared_ptr<Index> index)
            : Leg(paymentCalendar, nominal), index_{index} {};

    double getPayment(types::date from, types::date to) const {
        double fractionDate = dcfCalculator_(from, to);
        double rate = index_->calculateForwardRate(from, to);
        return nominal_ * rate * fractionDate;
    };

    double calculateDayFraction(types::date from, types::date to) const {
        return dcfCalculator_(from, to);
    }

private:
    DAY_COUNT_FRACTION dcfCalculator_;
    std::shared_ptr<Index> index_;
};

#endif