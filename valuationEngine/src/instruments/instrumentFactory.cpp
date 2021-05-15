#include "instrumentFactory.h"
#include "leg.h"
#include "fixedLeg.h"
#include "floatingLeg.h"
#include "bond.h"
#include "swap.h"

std::unique_ptr<instruments::IInstrument> instruments::InstrumentFactory::buildBond(double nominal,
                                                                                    double rate,
                                                                                    std::vector<types::date> &paymentCalendar,
                                                                                    types::Map curveData,
                                                                                    types::Conventions convention) {

    types::date today = paymentCalendar.at(0);

    typedef FixedLeg<day_count_fraction::Actual_360> FixedLegType; // todo:: modify
    auto myZeroCouponCurve = std::make_shared<market::ZeroCouponCurve>(curveData, today);
    auto myFixedLeg = std::unique_ptr<Leg>{
            std::make_unique<FixedLegType>(paymentCalendar, nominal, rate)
    };

    // Return an instrument of type bond
    return std::make_unique<instruments::Bond>(myFixedLeg, myZeroCouponCurve);
}

std::unique_ptr<instruments::IInstrument> instruments::InstrumentFactory::buildSwap(double nominal,
                                                                                    double rate,
                                                                                    float annualIndexFrequency,
                                                                                    const std::vector<types::date> &paymentCalendar,
                                                                                    const types::Map &curveData,
                                                                                    types::Conventions convention,
                                                                                    bool isReceiverFixedLeg) {

    types::date today = paymentCalendar.at(0);

    // Instance Fixed Leg
    typedef FixedLeg<day_count_fraction::Actual_360> FixedLegType; // todo:: modify
    auto myZeroCouponCurve = std::make_shared<market::ZeroCouponCurve>(curveData, today);
    auto myFixedLeg = std::unique_ptr<Leg>{
            std::make_unique<FixedLegType>(paymentCalendar, nominal, rate)
    };


    // Instance Floating leg
    typedef FloatingLeg<day_count_fraction::Actual_360> FloatingLegType; // todo:: modify
    auto myIndex = std::make_shared<market::Index>(annualIndexFrequency, myZeroCouponCurve);
    auto myFloatingLeg = std::unique_ptr<Leg>{
            std::make_unique<FloatingLegType>(paymentCalendar, nominal, myIndex)
    };

    // Receiver and payer
    std::unique_ptr<Leg> myReceiver;
    std::unique_ptr<Leg> myPayer;

    if (isReceiverFixedLeg) {
        myReceiver = std::move(myFixedLeg);
        myPayer = std::move(myFloatingLeg);
    } else {
        myPayer = std::move(myFixedLeg);
        myReceiver = std::move(myFloatingLeg);
    }

    // Return an instrument of type swap
    return std::make_unique<Swap>(myPayer, myReceiver, myZeroCouponCurve);
}
