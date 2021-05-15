#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include "instrument.h"
#include "../util/types.h"

namespace instruments {
    class InstrumentFactory {
    public:

        static std::unique_ptr<instruments::IInstrument> buildBond(double nominal,
                                                                   double rate,
                                                                   std::vector<types::date> &paymentCalendar,
                                                                   types::Map curveData,
                                                                   types::Conventions convention);


        static std::unique_ptr<instruments::IInstrument> buildSwap(double nominal,
                                                                   double rate,
                                                                   float annualIndexFrequency,
                                                                   const std::vector<types::date> &paymentCalendar,
                                                                   const types::Map &curveData,
                                                                   types::Conventions convention,
                                                                   bool isReceiverFixedLeg = true);


    };
}


#endif
