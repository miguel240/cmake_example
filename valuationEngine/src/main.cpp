#include <iostream>
#include <curves/index.h>
#include "dayCountFraction/dayCountCalculator.h"
#include "util/types.h"
#include "curves/zeroCouponCurve.h"


int main() {
    //std::unique_ptr<Index> zc(new Index(2.));
    //std:: cout << zc->getNum();

    types::date today = DayCountCalculator::make_date("2016/4/1");
    std::vector<types::date> paymentCalendar{
            today,
            DayCountCalculator::make_date("2016/10/3"),
            DayCountCalculator::make_date("2017/4/3"),
            DayCountCalculator::make_date("2017/10/2"),
            DayCountCalculator::make_date("2018/04/2")};


    types::Map zeroCurveData{
            {paymentCalendar[0], 1.},
            {paymentCalendar[1], 0.0474},
            {paymentCalendar[2], 0.05},
            {paymentCalendar[3], 0.051},
            {paymentCalendar[4], 0.052},
    };

    auto zeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);
    auto result = zeroCouponCurve->getDiscountCurve(paymentCalendar[1]);

    std::cout << result;

    return 0;
}
