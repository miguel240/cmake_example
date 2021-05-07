#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "common/types.h"

class Leg {
public:
    Leg();

    virtual types::payments getPayments() const;

    virtual double getNominal() const;

    virtual types::date getMaturity() const;

    virtual ~Leg();
};

#endif