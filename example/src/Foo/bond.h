#ifndef BOND_H
#define BOND_H

#include "leg.h"
#include "zeroCouponCurve.h"
#include <memory>


class Bond {
public:
    Bond(std::unique_ptr<Leg> &receiver,
         std::shared_ptr<ZeroCouponCurve> &zeroCouponCurve); //todo:mirar ref a shared

    double operator()() const;

private:
    std::unique_ptr<Leg> fixedLeg_;
    std::shared_ptr<ZeroCouponCurve> zeroCouponCurve_;
};

#endif
