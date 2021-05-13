//
// Created by javier on 13/5/21.
//

#ifndef VALUATIONENGINE_CALCULATETIR_H
#define VALUATIONENGINE_CALCULATETIR_H


class CalculateTIR {
public:
    CalculateTIR(double tolerance, double incremento,int iterations);
    double operator()(double nominal, double target,double cupon, double initial_value = 0.0))const ;
    virtual ~CalculateTIR();
private:
    double tolerance_;
    double incremento_;
    double iterations_;
};


#endif //VALUATIONENGINE_CALCULATETIR_H
