#include "bootstrapping.h"

types::MapDiscountCurveType market::Bootstrapping::operator()(const Bootstrapping::InstrumentMapType &instruments) const {
    types::MapDiscountCurveType curve;
    double annuity = 0;

    for (auto instrument : instruments) {
        curve.insert(std::make_pair(
                instrument.first,
                instrument.second->computeDiscountFactor(annuity))
        );
    }
    return curve;
}
