//
// Created by javier on 17/4/21.
//
#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>
#include <string>
//#include "boost/date_time/date_parsing.hpp"
//#include <boost/algorithm/string.hpp>

class DayCountCalculator
{
public://permite el acceso a la funcion dentro y fuera de la clase
    //funcion estatica que solo puede ser invocada desde el fichero en el que esta definida
    static boost::gregorian::date make_date(const std::string& date);//Cuando una función modifica un argumento que se
    // pasa por referencia, modifica el objeto original, no una copia local. Para evitar que una función modifique
    // este tipo de argumento, califique el parámetro como const&
    //el input es una fecha

};

boost::gregorian::date DayCountCalculator::make_date(const std::string &date) {
    return boost::gregorian::date(boost::gregorian::from_undelimited_string(date));
    //tranforma de string a tipo fecha boost pero no se si es correcto
}


class Actual_360 : public DayCountCalculator{//clase hija
public:
    static float compute_daycount(const std::string& from, const std::string& to);
    static float compute_daycount(const boost::gregorian::date & from,
                                  const boost::gregorian::date & to);

    template<class DATE>
    double operator () (const DATE& start, const DATE& end) const //no entiendo la sobrecarga de este operador
    {
        return compute_daycount(start, end)/ 360.0;
    }
};

float Actual_360::compute_daycount(const std::string& from, const std::string& to)
{
//parse from string + build date class

    //boost::gregorian::date from_date(boost::gregorian::from_string(from));
    //boost::gregorian::date to_date(boost::gregorian::from_string(to));
    //Se cambia el original por el siguiente: Preguntar Juan
    //boost::gregorian::date(boost::gregorian::from_undelimited_string(from));
    //ojo que con estos hay que poner la fecha "20200101"
    //boost::gregorian::date(boost::gregorian::from_undelimited_string(to));
    //boost::gregorian::date start, end;
    //start = boost::gregorian::date(boost::gregorian::from_undelimited_string(from));
    //Preguntar por la sobrecarga
    boost::gregorian::date start = make_date(from);
    boost::gregorian::date end = make_date(to);
    //boost::gregorian::date start = boost::gregorian::date(boost::gregorian::from_undelimited_string(from));
    //boost::gregorian::date end = boost::gregorian::date(boost::gregorian::from_undelimited_string(to));
    return  (end-start).days()/360.0;
}

float Actual_360::compute_daycount(const boost::gregorian::date & from,
                                   const boost::gregorian::date & to)
{
    return (to - from).days()/360.0;
}


class Thirty_360 : public DayCountCalculator{
public:
    static float compute_daycount(const std::string& from, const std::string& to);
    static auto compute_daycount(const boost::gregorian::date & from,
                                  const boost::gregorian::date & to);
    static float compute_daycount(const short years,
                                  const short months,
                                  const short days_from,
                                  const short days_to);
    template<class DATE>
    double operator () (const DATE& start, const DATE& end) const//igual con esta sobrecarga
    {
        return compute_daycount(start, end)/ 360.0;
    }
};

float Thirty_360::compute_daycount(const std::string& from, const std::string& to)
{
// parse from string + build date class + get a structure date structure.
    boost::gregorian::date start = make_date(from);
    boost::gregorian::date end = make_date(to);
        return (end-start).days()/360.0;
}//Esto hace lo mismo que el actual, la utima funcion es correcta

auto Thirty_360::compute_daycount(const boost::gregorian::date & from,
                                 const boost::gregorian::date & to)
{
    auto from_struct = from.year_month_day();
    auto to_struct = to.year_month_day();
    auto years = to_struct.year - from_struct.year;
    auto months = to_struct.month - from_struct.month;
    auto days = to_struct.day - from_struct.day;

    return ((360 * years) + 30 * (months) + days)/360.0;
}

//Esa funcion recibe años, meses dia inicial y final y calcula los dias total suponiendo cada mes como 30 dias
float Thirty_360::compute_daycount(const short years,const short months,
                                   const short days_from,
                                   const short days_to)
{
    return ((360 * years) + 30 * (months - 1) +
           std::max<short>(0, 30 - days_from) +//si en days_from se pone mas de 30 se va a 0
           std::min<short>(30, days_to))/360.0;//si ponemos mas de 30 se queda en 30

}

int main()
{

    //prueba1 con los dos formatos.
    Actual_360 test1;
    std::cout <<  test1.compute_daycount("19300101","19300201") << std::endl;
    std::cout <<  test1.compute_daycount(boost::gregorian::date(1930,01,01), boost::gregorian::date(1930,02,01)) << std::endl;
    //prueba2
    Thirty_360 test2;
    std::cout <<  test2.compute_daycount("19300101","19300201") << std::endl;//este no sale bien

    std::cout << test2.compute_daycount(boost::gregorian::date(1930,05,01),boost::gregorian::date(1930,06,01))<< std::endl;
    std::cout <<  test2.compute_daycount(0,1,01,01) << std::endl;
    



    return 0;

}
