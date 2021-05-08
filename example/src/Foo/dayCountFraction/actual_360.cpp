#include "actual_360.h"

float Actual_360::computeDaycount(const std::string &from, const std::string &to) {
    boost::gregorian::date start = make_date(from);
    boost::gregorian::date end = make_date(to);

    return (end - start).days() / 360.0;
}

float Actual_360::computeDaycount(const boost::gregorian::date &from,
                                   const boost::gregorian::date &to) {
    return (to - from).days() / 360.0;
}