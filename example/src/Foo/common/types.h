#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>

namespace types {
    typedef boost::gregorian::date date;
    typedef std::vector<std::pair<types::date, double>> payments;
}


#endif
