#ifndef DAY_COUNT_CALC_H
#define DAY_COUNT_CALC_H

#include "boost/date_time/gregorian/gregorian.hpp"

namespace day_count_fraction {
    class DayCountCalculator {
    public:
        // String (YYYY-MM-DD or YYYY/MM/DD) to gregorian date
        static boost::gregorian::date make_date(const std::string &date) {
            return boost::gregorian::from_string(date);
        }
    };
}

#endif

