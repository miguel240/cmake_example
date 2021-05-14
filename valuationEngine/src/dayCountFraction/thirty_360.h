#ifndef THIRTY_360_H
#define THIRTY_360_H

#include "dayCountCalculator.h"

namespace day_count_fraction {
    class Thirty_360 : public DayCountCalculator {
    public:

        static auto computeDaycount(const boost::gregorian::date &from,
                                    const boost::gregorian::date &to) {
            auto from_struct = from.year_month_day();
            auto to_struct = to.year_month_day();
            auto years = to_struct.year - from_struct.year;
            auto months = to_struct.month - from_struct.month;
            auto days = to_struct.day - from_struct.day;

            return ((360 * years) + 30 * (months) + days);
        }

        static float computeDaycount(const std::string &from, const std::string &to) {
            boost::gregorian::date start = make_date(from);
            boost::gregorian::date end = make_date(to);
            return Thirty_360::computeDaycount(start, end);
        }


        static float computeDaycount(const short years,
                                     const short months,
                                     const short days_from,
                                     const short days_to) {
            return ((360 * years) + 30 * (months - 1) +
                    std::max<short>(0, 30 - days_from) + //si en days_from se pone mas de 30 se va a 0
                    std::min<short>(30, days_to)); //si ponemos mas de 30 se queda en 30
        }

        template<class DATE>
        double operator()(const DATE &start, const DATE &end) const {
            return computeDaycount(start, end) / 360.0;
        }
    };
}

#endif