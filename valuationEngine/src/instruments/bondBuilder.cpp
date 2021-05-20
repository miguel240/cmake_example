#include "bondBuilder.h"
#include "factoryRegistrator.h"
#include "bond.h"
#include "fixedLeg.h"

namespace registerBond {
    FactoryRegistrator<instruments::BondBuilder> registration;
}

std::unique_ptr<instruments::IInstrument>
instruments::BondBuilder::build(const instruments::InstrumentDescription &instrument) {
    types::date today = instrument.receiver.paymentCalendar.at(0);

    // Create Zero Coupon Curve
    auto myZeroCouponCurve = std::make_shared<market::ZeroCouponCurve>(instrument.curveData, today);

    // Create Fixed Leg
    typedef instruments::FixedLeg<day_count_fraction::Actual_360> FixedLegType;
    auto myFixedLeg = std::unique_ptr<instruments::Leg>{
            std::make_unique<FixedLegType>(instrument.receiver.paymentCalendar,
                                           instrument.receiver.nominal,
                                           instrument.receiver.rate)
    };

    // Return instrument of type bond
    return std::unique_ptr<instruments::IInstrument>{
            std::make_unique<instruments::Bond>(myFixedLeg, myZeroCouponCurve)
    };
}

instruments::InstrumentDescription::Type instruments::BondBuilder::getId() {
    return instruments::InstrumentDescription::Type::bond;
}
