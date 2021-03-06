#ifndef ZERO_COUPON_CURVE_H
#define ZERO_COUPON_CURVE_H

#include "../util/types.h"
#include "dayCountFraction/actual_360.h"
#include <boost/optional/optional_io.hpp>

namespace market {
    class ZeroCouponCurve {
    public:
        ZeroCouponCurve(types::MapDiscountCurveType curveData, types::date today);

        boost::optional<double> getDiscountCurve(types::date date) const;

        boost::optional<double> getRate(types::date) const;

        void setFixedRate(double rate) {
            fixedRate_ = rate;
        }

        types::date getTodayDate() const {
            return today_;
        }

    private:
        types::MapDiscountCurveType curveData_;
        types::date today_;
        day_count_fraction::Actual_360 dcfCalculator_;
        boost::optional<double> fixedRate_;
    };
}

#endif
