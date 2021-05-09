#ifndef UTILS_H
#define UTILS_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <unordered_map>
#include "types.h"

struct dateComparator {
    bool operator()(const boost::gregorian::date &date1,
                    const boost::gregorian::date &date2) const {
        return date1 > date2;
    }
};

#endif
