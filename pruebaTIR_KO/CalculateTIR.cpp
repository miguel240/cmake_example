//
// Created by javier on 13/5/21.
//

#include "CalculateTIR.h"
CalculateTIR::~CalculateTIR() {};

CalculateTIR::CalculateTIR(double tolerance, double incremento, int iterations):
        tolerance_(tolerance),
        incremento_(incremento),
        iterations_(iterations)
{}


double CalculateTIR::operator()(double nominal, double target,double cupon, double initial_value = 0.0)const {
    types::date today = DayCountCalculator::make_date("2016/4/1");
    std::vector<types::date> paymentCalendar{
            today,
            DayCountCalculator::make_date("2016/10/3"),
            DayCountCalculator::make_date("2017/4/3"),
            DayCountCalculator::make_date("2017/10/2"),
            DayCountCalculator::make_date("2018/04/2")};
    for (int i = 0; i < iterations_; ++i) {
        types::Map zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], initial_value},
                {paymentCalendar[2], initial_value},
                {paymentCalendar[3], initial_value},
                {paymentCalendar[4], initial_value},
        };

        typedef FixedLeg<Actual_360> FixedLegType;
        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, nominal, cupon)
        };

        auto myBond = Bond(myFixedLeg, myZeroCouponCurve);
        double f_y = myBond();
        if (std::abs(f_y) < tolerance_)
        {
            break;
        }
        initial_value = initial_value + incremento_;
        types::Map zeroCurveData_dy{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], initial_value},
                {paymentCalendar[2], initial_value},
                {paymentCalendar[3], initial_value},
                {paymentCalendar[4], initial_value},
        };

        typedef FixedLeg<Actual_360> FixedLegType;
        auto myZeroCouponCurve_dy = std::make_shared<ZeroCouponCurve>(zeroCurveData_dy, today);
        auto myFixedLeg_dy = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, nominal, cupon)
        };
        auto myBond_dy = Bond(myFixedLeg_dy, myZeroCouponCurve_dy);
        double f_inc = myBond();
        auto f_dy = (f_inc-target )/incremento_;
        y = y - (f_y/f_dy);
    }
    return y;
}
