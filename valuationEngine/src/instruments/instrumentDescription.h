#ifndef INSTRUMENT_DESCRIPTION_H
#define INSTRUMENT_DESCRIPTION_H

#include "vector"
#include "util/types.h"

namespace instruments{

    struct LegDescription {
        std::vector<types::date> paymentCalendar{};
        bool isFixed = false;
        double nominal{};
        double rate{};
        int indexFrequency = 0;
    };

    struct InstrumentDescription {
        enum Type { deposit, bond, swap };
        Type type;
        InstrumentDescription(Type type_) : type{type_} {}
        types::MapDiscountCurveType curveData{};
        LegDescription receiver{}; // Fixed Leg in a bond
        LegDescription payer{};
    };

}

#endif
