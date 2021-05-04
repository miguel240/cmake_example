#include "thirty_360.h"

 auto Thirty_360::compute_daycount(const boost::gregorian::date &from,
                                  const boost::gregorian::date &to) {

    auto from_struct = from.year_month_day();
    auto to_struct = to.year_month_day();
    auto years = to_struct.year - from_struct.year;
    auto months = to_struct.month - from_struct.month;
    auto days = to_struct.day - from_struct.day;

    return ((360 * years) + 30 * (months) + days) / 360.0;
}

float Thirty_360::compute_daycount(const std::string &from, const std::string &to) {
    boost::gregorian::date start = make_date(from);
    boost::gregorian::date end = make_date(to);
    return Thirty_360::compute_daycount(start, end);
}

// Esa funcion recibe años, meses dia inicial y
// final y calcula los dias total suponiendo cada mes como 30 dias
float Thirty_360::compute_daycount(const short years, const short months,
                                   const short days_from,
                                   const short days_to) {
    return ((360 * years) + 30 * (months - 1) +
            std::max<short>(0, 30 - days_from) + //si en days_from se pone mas de 30 se va a 0
            std::min<short>(30, days_to)) / 360.0; //si ponemos mas de 30 se queda en 30
}