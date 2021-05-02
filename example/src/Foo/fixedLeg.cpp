#include "fixedLeg.h"

fixedLeg::FixedLeg(float interes) : interes_{interes} {};


double fixedLeg::getPrice() {
    double price;

    for (auto fechaPago : calendarioPagos_) {
        price += exp(- interes * getFraccion(today, fechaPagoo, '360')); // e^-rt
    }
}


