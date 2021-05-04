#include "instrument.h"

Instrument::Instrument() {}

void Instrument::buildInstrument(std::string type) {
    if (type == "bond") {
        const std::map<std::string, float> zeroCurveData = {{"20201003", 4.74},
                                                            {"20170403", 5},
                                                            {"20171002", 5.1},
                                                            {"20180402", 5.2}};

        //auto zeroCouponCurve = std::make_unique<ZeroCouponCurve>(zeroCurveData); // deberia pasar por referencia ?
    }
}

int main() {
    std::cout << "In real main\n";
    std::cout << "Exit real main\n";
}