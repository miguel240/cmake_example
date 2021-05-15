#include "goalSeeker.h"

GoalSeeker::GoalSeeker(double tolerance, double increment, unsigned long maxIterations) :
        tolearance_{tolerance}, increment_{increment}, maxIterations_{maxIterations} {}

double GoalSeeker::operator()(std::function<double(double)> func,
                              double target,
                              double initialValue) const {

    auto funcEqualToZero = [func, target](double value) { return func(value) - target; };

    double y = initialValue;
    for (int i = 0; i < maxIterations_; ++i) {
        double fy = funcEqualToZero(y);

        // TIR encontrada
        if (std::abs(fy) < tolearance_) break;

        double dfy = (funcEqualToZero(y + increment_) - fy) / increment_;
        y = y - (fy / dfy);
    }

    return y;
}
