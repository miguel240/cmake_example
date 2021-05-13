#ifndef SWAP_H
#define SWAP_H

#include "leg.h"
#include "market/zeroCouponCurve.h"

class Swap {
public:
    Swap(std::unique_ptr<Leg> &payer, std::unique_ptr<Leg> &receiver,
         std::shared_ptr<ZeroCouponCurve> zeroCouponCurve);

    double operator()() const;

private:
    double calculateTotal_(const types::payments &payments) const;
    double calculatePresentValue_(types::date date, double value) const;

    // Variables
    std::unique_ptr<Leg> payer_;
    std::unique_ptr<Leg> receiver_;
    std::shared_ptr<ZeroCouponCurve> zeroCouponCurve_;
};

#endif
