#ifndef BOND_H
#define BOND_H

#include "instrument.h"
#include "leg.h"
#include "market/zeroCouponCurve.h"
#include <memory>

namespace instruments {
    class Bond : public IInstrument {
    public:
        Bond(std::unique_ptr<Leg> &fixedLeg,
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
