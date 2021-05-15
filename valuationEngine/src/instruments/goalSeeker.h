#ifndef GOAL_SEEKER_H
#define GOAL_SEEKER_H


#include <functional>

class GoalSeeker {
public:
    GoalSeeker(double tolerance, double increment, unsigned long maxIterations);

    double operator()(std::function<double(double)> func, double target, double initialValue = 0) const;

private:
    double tolearance_;
    double increment_;
    unsigned long maxIterations_;
};


#endif
