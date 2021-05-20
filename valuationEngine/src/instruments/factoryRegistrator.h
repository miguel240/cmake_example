#ifndef INSTRUMENT_REGISTRATOR_H
#define INSTRUMENT_REGISTRATOR_H

#include "instrumentFactory.h"

template<typename BuilderClass>
class FactoryRegistrator {
public:
    FactoryRegistrator() {
        instruments::InstrumentFactory::instance().registerConstructor(BuilderClass::getId(), &BuilderClass::build);
    }
};

#endif
