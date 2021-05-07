#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>

class Leg {
public:
    Leg();

    virtual std::vector<std::pair<boost::gregorian::date, double>> getCalendarWithPayments() ;

    virtual ~Leg();
};

#endif