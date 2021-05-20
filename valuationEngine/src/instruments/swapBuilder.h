#ifndef SWAP_BUILDER_H
#define SWAP_BUILDER_H

#include <memory>
#include <util/types.h>
#include "instrumentFactory.h"

namespace instruments {
    class SwapBuilder {
    public:
        static std::unique_ptr<instruments::IInstrument> build(const instruments::InstrumentDescription &instrument);

        static instruments::InstrumentDescription::Type getId();
    };
}

#endif
