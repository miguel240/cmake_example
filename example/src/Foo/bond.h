#ifndef BOND_H
#define BOND_H

#include "leg.h"
#include "zeroCouponCurve.h"
#include <memory>
#include "dayCountFraction/actual_360.h"


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
    Actual_360 dcfCalculator_; // todo: pasarle la del fixed leg
};

#endif
