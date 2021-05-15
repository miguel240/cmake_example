#ifndef TYPES_H
#define TYPES_H

#include "comparators.h"
#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <map>

namespace types {
    enum Conventions {
        Actual360, Thirty360
    };

    typedef boost::gregorian::date date;
    typedef std::vector<std::pair<types::date, double>> payments;
    typedef std::map<types::date, double> Map;
    typedef std::pair<types::date, double> curvePair;
}

#endif
