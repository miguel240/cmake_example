#ifndef FLOATING_LEG_H
#define FLOATING_LEG_H

#include "dayCountFraction/dayCountCalculator.h"
#include "leg.h"
#include "../market/index.h"

namespace instruments {
    template<class DAY_COUNT_FRACTION>
    class FloatingLeg : public Leg {
    public:
        FloatingLeg(const std::vector<types::date> &paymentCalendar,
                    double nominal,
                    std::shared_ptr<market::Index> index)
                : Leg(paymentCalendar, nominal), index_{index} {};

        virtual double getPayment(const types::date &from, const types::date &to) const {
            double fractionDate = dcfCalculator_(from, to);
            double rate = index_->calculateForwardRate(from, to);
            return nominal_ * rate * fractionDate;
        };

        virtual double calculateDayFraction(types::date from, types::date to) const {
            return dcfCalculator_(from, to);
        }

        ~FloatingLeg() {};

    private:
        DAY_COUNT_FRACTION dcfCalculator_;
        std::shared_ptr<market::Index> index_;
    };
}

#endif
