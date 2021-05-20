#include "depositBuilder.h"
#include "factoryRegistrator.h"
#include "deposit.h"
#include "fixedLeg.h"

namespace registerDeposit{
    FactoryRegistrator<instruments::DepositBuilder> registration;
}

std::unique_ptr<instruments::IInstrument>
instruments::DepositBuilder::build(const instruments::InstrumentDescription &instrument) {
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
            std::make_unique<instruments::Deposit>(myFixedLeg, myZeroCouponCurve)
    };
}

instruments::InstrumentDescription::Type instruments::DepositBuilder::getId() {
    return instruments::InstrumentDescription::Type::deposit;
}
