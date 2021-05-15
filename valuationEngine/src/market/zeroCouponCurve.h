#ifndef ZERO_COUPON_CURVE_H
#define ZERO_COUPON_CURVE_H

#include "../util/types.h"
#include "dayCountFraction/actual_360.h"
#include <boost/optional/optional_io.hpp>

namespace market {
    class ZeroCouponCurve {
    public:
        ZeroCouponCurve(types::Map curveData, types::date today);

        boost::optional<double> getDiscountCurve(types::date date) const;

        boost::optional<double> getRate(types::date) const;

        void setFixedRate(double rate) {
            isFixedRate = true;
            fixedRate_ = rate;
        }

        types::date getTodayDate() const {
            return today_;
        }

    private:
        // Returns an element from the map
        boost::optional<types::curvePair> getZeroCouponElement_(const types::date date) const;

        // Variables
        types::Map curveData_;
        types::date today_;
        day_count_fraction::Actual_360 dcfCalculator_;
        double fixedRate_;
        bool isFixedRate = false;
    };
}

#endif
