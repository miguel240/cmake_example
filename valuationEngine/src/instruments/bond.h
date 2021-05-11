#ifndef BOND_H
#define BOND_H

#include "leg.h"
#include "curves/zeroCouponCurve.h"
#include <memory>


class Bond {
public:
    Bond(std::unique_ptr<Leg> &fixedLeg,
         std::shared_ptr<ZeroCouponCurve> zeroCouponCurve);

    double operator()() const;

private:
    double calculatePresentValue(types::date date, double value, float numOfPaymentsInAYear) const;
    float getNumOfPaymentsInAYear(const std::vector<types::date> &paymentCalendar) const;

    // Variables
    std::unique_ptr<Leg> fixedLeg_;
    std::shared_ptr<ZeroCouponCurve> zeroCouponCurve_;
};

#endif
