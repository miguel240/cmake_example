#ifndef FIXEDLEG_H
#define FIXEDLEG_H

#include "boost/date_time/gregorian/gregorian.hpp"


class FixedLeg : public Leg {
public:
    FixedLeg(float interes);

    double getPrice();

private:
    std::vector<boost::gregorian::date> calendarioPagos_{boost::gregorian::date(2016, 10, 03),
                                                         boost::gregorian::date(2016, 04, 03),
                                                         boost::gregorian::date(2017, 10, 02),
                                                         boost::gregorian::date(2017, 04, 02)};
    float interes_;
    auto today{boost::gregorian::date(2016, 04, 01)};
};

#endif
