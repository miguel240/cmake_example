#ifndef SWAP_H
#define SWAP_H

#include "instrument.h"
#include "leg.h"
#include "market/zeroCouponCurve.h"

namespace instruments {
    class Swap : public IInstrument {
    public:
        Swap(std::unique_ptr<Leg> &payer, std::unique_ptr<Leg> &receiver,
             std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve);

        double operator()() const;

        double computeDiscountFactor(double &preAnnuity) const ;

    private:
        double calculateTotal_(const types::payments &payments) const;

        double calculatePresentValue_(types::date date, double value) const;

        // Variables
        std::unique_ptr<Leg> payer_;
        std::unique_ptr<Leg> receiver_;
        std::shared_ptr<market::ZeroCouponCurve> zeroCouponCurve_;
    };
}


#endif
