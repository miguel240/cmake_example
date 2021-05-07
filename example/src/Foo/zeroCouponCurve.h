#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include <map>
#include <boost/date_time/gregorian/greg_date.hpp>

class ZeroCouponCurve {
public:
    ZeroCouponCurve(const std::map<boost::gregorian::date, double> curveData,
                    boost::gregorian::date today);

    double getZeroCouponRate(boost::gregorian::date date) const; // todo: revisar si se usa

    double getDiscountCurve(boost::gregorian::date date) const;

private:
    std::map<boost::gregorian::date, double> curveData_;
    boost::gregorian::date today_;
};

#endif
