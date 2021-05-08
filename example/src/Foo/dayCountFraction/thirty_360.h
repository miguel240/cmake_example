#ifndef THIRTY_360_H
#define THIRTY_360_H

#include "dayCountCalculator.h"

class Thirty_360 : public DayCountCalculator {
public:
    static float computeDaycount(const std::string &from, const std::string &to);

    static auto computeDaycount(const boost::gregorian::date &from,
                                const boost::gregorian::date &to);

    static float computeDaycount(const short years,
                                 const short months,
                                 const short days_from,
                                 const short days_to);

    template<class DATE>
    double operator()(const DATE &start, const DATE &end) const {
        return computeDaycount(start, end) / 360.0;
    }
};

#endif