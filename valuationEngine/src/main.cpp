#include <iostream>
#include "instruments/instrumentFactory.h"
#include "dayCountFraction/dayCountCalculator.h"
#include "util/types.h"

int main() {
    std::vector<types::date> paymentCalendar{
            day_count_fraction::DayCountCalculator::make_date("2016/4/1"),
            day_count_fraction::DayCountCalculator::make_date("2016/10/3"),
            day_count_fraction::DayCountCalculator::make_date("2017/4/3"),
            day_count_fraction::DayCountCalculator::make_date("2017/10/2"),
            day_count_fraction::DayCountCalculator::make_date("2018/04/2")};

    types::MapDiscountCurveType zeroCurveData{
            {paymentCalendar[0], 1.},
            {paymentCalendar[1], 0.05},
            {paymentCalendar[2], 0.058},
            {paymentCalendar[3], 0.064},
            {paymentCalendar[4], 0.068},
    };

    // Create Bond
    auto myBond = instruments::InstrumentFactory::buildBond(100,
                                                            0.06,
                                                            paymentCalendar,
                                                            zeroCurveData,
                                                            types::Conventions::Actual360);

    // Create Swap
    auto mySwap = instruments::InstrumentFactory::buildSwap(100,
                                                            0.06,
                                                            2,
                                                            paymentCalendar,
                                                            zeroCurveData,
                                                            types::Conventions::Actual360);

    // Output
    std::cout << "Bond Price: " << myBond->operator()() << "\n";
    std::cout << "Swap Price: " << mySwap->operator()();
}



