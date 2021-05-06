#ifndef LEG_H
#define LEG_H

#include <string>
#include <vector>

class Leg {
public:
    //Leg();

    virtual std::vector<std::pair<std::string, double>> getCalendarWithPayments() const = 0;

    virtual ~Leg();
};

#endif