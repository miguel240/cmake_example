#include "instrumentFactory.h"


instruments::InstrumentFactory::InstrumentFactory() {} // Private


std::unique_ptr<instruments::IInstrument>
    instruments::InstrumentFactory::operator()(const InstrumentDescription &description) const {

    auto builder = buildersMap_.find(description.type);

    if (builder == buildersMap_.end()) {
        throw std::runtime_error("Invalid payoff descriptor");
    }

    return (builder->second)(description); // Call Build Function
}

// Add [instrument: build function] to the map
void instruments::InstrumentFactory::registerConstructor(const InstrumentDescription::Type &id,
                                                         const InstrumentFactory::Builder &builder) {

    buildersMap_.insert(std::make_pair(id, builder));
}

