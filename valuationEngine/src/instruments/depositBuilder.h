#ifndef DEPOSIT_BUILDER_H
#define DEPOSIT_BUILDER_H

#include <memory>
#include "instrument.h"
#include "instrumentDescription.h"

namespace instruments {
    class DepositBuilder {
    public:
        static std::unique_ptr<instruments::IInstrument> build(const instruments::InstrumentDescription &instrument);

        static instruments::InstrumentDescription::Type getId();
    };
};

#endif
