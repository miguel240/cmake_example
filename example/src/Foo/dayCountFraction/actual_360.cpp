#include "actual_360.h"

float Actual_360::compute_daycount(const std::string &from, const std::string &to) {
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
    return (end - start).days() / 360.0;
}

float Actual_360::compute_daycount(const boost::gregorian::date &from,
                                   const boost::gregorian::date &to) {
    return (to - from).days() / 360.0;
}