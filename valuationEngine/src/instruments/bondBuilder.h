#ifndef BOND_BUILDER_H
#define BOND_BUILDER_H

#include <memory>
#include <util/types.h>
#include "instrumentFactory.h"

namespace instruments {
    class BondBuilder {
    public:
        static std::unique_ptr<instruments::IInstrument> build(const instruments::InstrumentDescription &instrument);

        static instruments::InstrumentDescription::Type getId();
    };

}
#endif
