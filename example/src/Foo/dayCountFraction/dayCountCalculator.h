#ifndef DAY_COUNT_CALC_H
#define DAY_COUNT_CALC_H

#include "boost/date_time/gregorian/gregorian.hpp"

class DayCountCalculator {
public:
    static boost::gregorian::date make_date(const std::string &date) {
        return boost::gregorian::date(boost::gregorian::from_undelimited_string(date));
    }
};

#endif

