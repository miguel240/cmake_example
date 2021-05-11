#ifndef ACTUAL_360_H
#define ACTUAL_360_H

#include "dayCountCalculator.h"

class Actual_360 : public DayCountCalculator {
public:
    static float computeDaycount(const std::string &from, const std::string &to){
        boost::gregorian::date start = make_date(from);
        boost::gregorian::date end = make_date(to);

        return (end - start).days();
    }

    static float computeDaycount(const boost::gregorian::date &from,
                                  const boost::gregorian::date &to){
        return (to - from).days();
    }

    template<class DATE>
    double operator()(const DATE &start, const DATE &end) const {
        return computeDaycount(start, end) / 360.0;
    }
};


#endif
