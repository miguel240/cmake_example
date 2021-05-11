#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include "types.h"

struct dateComparator {
    bool operator()(const boost::gregorian::date &date1,
                    const boost::gregorian::date &date2) const {
        return date1 < date2;
    }
};

#endif
