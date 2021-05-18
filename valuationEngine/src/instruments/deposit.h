#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "instrument.h"
#include "leg.h"

namespace instruments {
    class Deposit : public IInstrument {
    public:
        Deposit(std::unique_ptr<Leg> &fixedLeg,
                std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve);

        double operator()() const;

        double computeDiscountFactor(double &preAnnuity) const;


    private:
        double calculatePresentValue_(types::date date, double value) const;

        // Variables
        std::unique_ptr<Leg> fixedLeg_;
        std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve_;
    };
}

#endif
