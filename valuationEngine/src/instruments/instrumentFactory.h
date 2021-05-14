#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include "instrument.h"

namespace instruments {
    class InstrumentFactory {
    public:

        static std::unique_ptr<instruments::IInstrument> buildBond(double nominal,
                                                                   double rate,
                                                                   std::vector<types::date> &paymentCalendar,
                                                                   types::Map curveData);


        static std::unique_ptr<instruments::IInstrument> buildSwap(double nominal,
                                                                   double rate,
                                                                   float annualIndexFrequency,
                                                                   const std::vector<types::date> &paymentCalendar,
                                                                   const types::Map &curveData,
                                                                   bool isReceiverFixedLeg = true);
    };
}


#endif
