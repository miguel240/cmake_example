#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include "instrumentDescription.h"
#include "instruments/instrument.h"

namespace instruments {

    class InstrumentFactory {
    public:
        std::unique_ptr<IInstrument> operator()(const InstrumentDescription &description) const;

        typedef std::function<std::unique_ptr<IInstrument>(const InstrumentDescription &)> Builder;

        void registerConstructor(const InstrumentDescription::Type &id, const Builder &builder);

        // Singleton
        static InstrumentFactory &instance() {
            static InstrumentFactory factory;
            return factory;
        }

        virtual ~InstrumentFactory() {};
    private:
        InstrumentFactory(); // Force to call instance()

        std::map<InstrumentDescription::Type, Builder> buildersMap_;
    };

}

#endif
