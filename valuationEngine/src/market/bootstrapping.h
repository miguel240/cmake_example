#ifndef BOOTSRAPPING_H
#define BOOTSRAPPING_H

#include <instruments/instrument.h>
#include "util/types.h"

class Bootstrapping {
public:
    Bootstrapping() {};

    typedef std::map<types::date, std::shared_ptr<instruments::IInstrument>> InstrumentMapType;

    types::MapDiscountCurveType operator()(const InstrumentMapType &instruments) const;

    virtual ~Bootstrapping() {};
};


#endif
