#ifndef FIXEDLEG_H
#define FIXEDLEG_H

#include <vector>
#include <string>
#include "leg.h"

class FixedLeg : public leg {
public:
    FixedLeg(std::vector<std::string> paymentCalendar, float interest, float nominal); // el vector se pasa por ref ?

    float getPrice();

private:
    std::vector<std::pair<std::string, float>> payments_; // refactor nombre
    std::vector<std::string> paymentCalendar_;
    float interest_;
    float nominal_;
};

#endif
