#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <util/types.h>

namespace instruments {
    class IInstrument {
    public:
        virtual double operator()() const = 0;

        virtual ~IInstrument() {};

    private:
        virtual double calculatePresentValue_(types::date date, double value) const = 0;
    };
}

#endif
