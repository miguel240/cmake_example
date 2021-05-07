#ifndef DAYCOUNTCALC_H
#define DAYCOUNTCALC_H

#include "boost/date_time/gregorian/gregorian.hpp"


//#include "boost/date_time/date_parsing.hpp"
//#include <boost/algorithm/string.hpp>

class DayCountCalculator {
public://permite el acceso a la funcion dentro y fuera de la clase
    //funcion estatica que solo puede ser invocada desde el fichero en el que esta definida
    static boost::gregorian::date make_date(const std::string &date) {
        return boost::gregorian::date(boost::gregorian::from_undelimited_string(date));
    }

    //Cuando una función modifica un argumento que se
    // pasa por referencia, modifica el objeto original, no una copia local. Para evitar que una función modifique
    // este tipo de argumento, califique el parámetro como const&
    //el input es una fecha

};

#endif

