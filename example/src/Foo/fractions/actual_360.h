#ifndef ACTUAL_360_H
#define ACTUAL_360_H

#include "dayCountCalculator.h"


class Actual_360 : public DayCountCalculator {//clase hija
public:
    static float compute_daycount(const std::string &from, const std::string &to);

    static float compute_daycount(const boost::gregorian::date &from,
                                  const boost::gregorian::date &to);

    template<class DATE>
    double operator()(const DATE &start, const DATE &end) const // no entiendo la sobrecarga de este operador
    {
        return compute_daycount(start, end) / 360.0;
    }
};


#endif
