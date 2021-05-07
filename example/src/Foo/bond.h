#ifndef BOND_H
#define BOND_H

#include "leg.h"
#include "zeroCouponCurve.h"
#include <memory>


class Bond {
public:
    Bond(std::unique_ptr<Leg> &fixedLeg,
         std::shared_ptr<ZeroCouponCurve> zeroCouponCurve);

    double operator()() const;

private:
    double calculatePresentValue(types::date date, double value) const;

    // Variables
    std::unique_ptr<Leg> fixedLeg_;
    std::shared_ptr<ZeroCouponCurve> zeroCouponCurve_;
};

#endif
