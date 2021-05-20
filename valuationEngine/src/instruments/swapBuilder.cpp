#include "swapBuilder.h"
#include "factoryRegistrator.h"
#include "swap.h"
#include "fixedLeg.h"
#include "floatingLeg.h"

namespace registerSwap {
    FactoryRegistrator<instruments::SwapBuilder> registration;
}

std::unique_ptr<instruments::IInstrument>
instruments::SwapBuilder::build(const instruments::InstrumentDescription &instrument) {
    types::date today = instrument.payer.paymentCalendar.at(0);

    // Create Zero Coupon Curve
    auto myZeroCouponCurve = std::make_shared<market::ZeroCouponCurve>(instrument.curveData, today);


    // Create Fixed Leg
    typedef instruments::FixedLeg<day_count_fraction::Actual_360> FixedLegType;
    auto myFixedLeg = std::unique_ptr<instruments::Leg>{
            std::make_unique<FixedLegType>(instrument.receiver.paymentCalendar,
                                           instrument.receiver.nominal,
                                           instrument.receiver.rate)
    };

    // Instance Floating leg
    typedef FloatingLeg<day_count_fraction::Actual_360> FloatingLegType;
    auto myIndex = std::make_shared<market::Index>(instrument.payer.indexFrequency, myZeroCouponCurve);
    auto myFloatingLeg = std::unique_ptr<Leg>{
            std::make_unique<FloatingLegType>(instrument.payer.paymentCalendar, instrument.payer.nominal, myIndex)
    };

    // Return instrument of type swap
    return std::unique_ptr<instruments::IInstrument>{
            std::make_unique<instruments::Swap>(myFixedLeg, myFloatingLeg, myZeroCouponCurve)
    };
}

instruments::InstrumentDescription::Type instruments::SwapBuilder::getId() {
    return instruments::InstrumentDescription::Type::swap;
}
