#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "util/types.h"

#include "dayCountFraction/dayCountCalculator.h"
#include "dayCountFraction/actual_360.h"
#include "dayCountFraction/thirty_360.h"

#include "instruments/leg.h"
#include "instruments/fixedLeg.h"
#include "instruments/floatingLeg.h"

#include <market/zeroCouponCurve.h>
#include <market/index.h>
#include <instruments/bond.h>
#include <instruments/swap.h>


BOOST_AUTO_TEST_SUITE(day_count_fraction)

    BOOST_AUTO_TEST_CASE(test_make_date_boost)
    {
        BOOST_TEST_MESSAGE("Parsing string date to boost date");

        auto parseStringToGregorianFunc = []() {
            try {
                DayCountCalculator::make_date("1994-02-22");
            } catch (int _) {
                throw std::logic_error("Format error");
            }
        };

        BOOST_CHECK_NO_THROW(parseStringToGregorianFunc());
    }

    BOOST_AUTO_TEST_CASE(test_Actual_360)
    {
        BOOST_TEST_MESSAGE("Testing Actual_360 convention");
        auto dcfCalculator = Actual_360();

        std::string from{"2016/4/1"};
        std::string to{"2016/10/3"};

        auto yearFraction = dcfCalculator(from, to);
        //BOOST_TEST_MESSAGE(yearFraction);

        BOOST_TEST(yearFraction == 0.5138888888888888, boost::test_tools::tolerance(1e-11));
    }

    BOOST_AUTO_TEST_CASE(test_Thirty_360)
    {
        BOOST_TEST_MESSAGE("Testing Thirty_360 convention");
        auto dcfCalculator = Thirty_360();

        std::string from{"2016/4/1"};
        std::string to{"2016/10/3"};

        auto yearFraction = dcfCalculator(from, to);
        //BOOST_TEST_MESSAGE(yearFraction);

        BOOST_TEST(yearFraction == 0.5055555555555555, boost::test_tools::tolerance(1e-10));
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(index_)

    BOOST_AUTO_TEST_CASE(test_forward_Rate) {

        BOOST_TEST_MESSAGE("Testing Index forward rate");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        types::Map zeroCurveData{
                {today,                                      1.},
                {DayCountCalculator::make_date("2016/10/3"), 0.0474},
                {DayCountCalculator::make_date("2017/4/3"),  0.05},
                {DayCountCalculator::make_date("2017/10/2"), 0.051},
                {DayCountCalculator::make_date("2018/04/2"), 0.052},
        };


        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);
        auto myIndex = Index(2, myZeroCouponCurve);
        double forwardRate = myIndex.calculateForwardRate(DayCountCalculator::make_date("2016/10/3"),
                                                          DayCountCalculator::make_date("2017/4/3"));

        BOOST_TEST(forwardRate == 0.0533, boost::test_tools::tolerance(1e-3));
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(legs)

    BOOST_AUTO_TEST_CASE(test_fixed_leg_maturity)
    {
        BOOST_TEST_MESSAGE("Testing Fixed Leg Maturity");

        std::vector<types::date> paymentCalendar{
                DayCountCalculator::make_date("2016/4/1"), // Today
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        typedef FixedLeg<Actual_360> FixedLegType;
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.05)
        };

        types::date maturityDate = myFixedLeg->getMaturity();

        BOOST_TEST(maturityDate == DayCountCalculator::make_date("2018/04/2"));
    };


    BOOST_AUTO_TEST_CASE(test_fixed_leg_payments)
    {
        BOOST_TEST_MESSAGE("Testing Fixed Leg");

        std::vector<types::date> paymentCalendar{
                DayCountCalculator::make_date("2016/4/1"), // Today
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        typedef FixedLeg<Actual_360> FixedLegType;
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.05)
        };

        auto payments = myFixedLeg->getPayments();

        std::vector<types::date> paymentsDates;
        std::vector<double> paymentValues;

        for (auto payment : payments) {
            paymentsDates.push_back(payment.first);
            paymentValues.push_back(payment.second);
        }

        std::vector<types::date> correctDates(paymentCalendar.begin() + 1, paymentCalendar.end());
        std::vector<double> correctValues{2.57, 2.53, 2.53, 2.53};

        // CHECK PAYMENTS DATES
        BOOST_CHECK_EQUAL_COLLECTIONS(paymentsDates.begin(), paymentsDates.end(),
                                      correctDates.begin(), correctDates.end());

        // CHECK PAYS
        for (int i = 0; i < paymentValues.size(); i++) {
            BOOST_TEST(paymentValues.at(i) == correctValues.at(i), boost::test_tools::tolerance(1e-3));
        }
    };

    BOOST_AUTO_TEST_CASE(test_floating_leg)
    {
        BOOST_TEST_MESSAGE("Testing Floating Leg payments");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::Map zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474},
                {paymentCalendar[2], 0.05},
                {paymentCalendar[3], 0.051},
                {paymentCalendar[4], 0.052},
        };
        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);
        auto myIndex = std::make_shared<Index>(2, myZeroCouponCurve);

        typedef FloatingLeg<Actual_360> FloatingLegType;
        auto myFloatingLeg = std::unique_ptr<Leg>{
                std::make_unique<FloatingLegType>(paymentCalendar, 100, myIndex)
        };

        auto payments = myFloatingLeg->getPayments();

        std::vector<types::date> paymentsDates;
        std::vector<double> paymentValues;

        for (auto payment : payments) {
            paymentsDates.push_back(payment.first);
            paymentValues.push_back(payment.second);
        }

        std::vector<types::date> correctDates(paymentCalendar.begin() + 1, paymentCalendar.end());
        std::vector<double> correctValues{2.467, 2.695, 2.716, 2.820};

        // CHECK PAYMENTS DATES
        BOOST_CHECK_EQUAL_COLLECTIONS(paymentsDates.begin(), paymentsDates.end(),
                                      correctDates.begin(), correctDates.end());

        // CHECK PAYS
        for (int i = 0; i < paymentValues.size(); i++) {
            BOOST_TEST(paymentValues.at(i) == correctValues.at(i), boost::test_tools::tolerance(1e-3));
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(zero_coupon)

    BOOST_AUTO_TEST_CASE(test_zc_date)
    {
        BOOST_TEST_MESSAGE("Testing Zero Coupon discount curve");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};


        types::Map zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474},
                {paymentCalendar[2], 0.05},
                {paymentCalendar[3], 0.051},
                {paymentCalendar[4], 0.052},
        };

        auto myZeroCouponCurve = std::make_unique<ZeroCouponCurve>(zeroCurveData, today);
        auto zcRate = myZeroCouponCurve->getDiscountCurve(paymentCalendar.at(1));

        BOOST_TEST(*zcRate == 0.97593577, boost::test_tools::tolerance(1e-6));
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(instruments)

    BOOST_AUTO_TEST_CASE(test_bond)
    {
        BOOST_TEST_MESSAGE("Testing Bond instrument");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::Map zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.05},
                {paymentCalendar[2], 0.058},
                {paymentCalendar[3], 0.064},
                {paymentCalendar[4], 0.068},
        };

        typedef FixedLeg<Actual_360> FixedLegType;
        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.06)
        };

        auto myBond = Bond(myFixedLeg, myZeroCouponCurve);
        BOOST_TEST_MESSAGE(myBond());
    }

    BOOST_AUTO_TEST_CASE(test_swap) {

        BOOST_TEST_MESSAGE("Testing Swap instrument");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::Map zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474},
                {paymentCalendar[2], 0.05},
                {paymentCalendar[3], 0.051},
                {paymentCalendar[4], 0.052},
        };
        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);

        // Instance Fixed Leg
        typedef FixedLeg<Actual_360> FixedLegType;
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.06)
        };

        // Instance Floating leg
        typedef FloatingLeg<Actual_360> FloatingLegType;
        auto myIndex = std::make_shared<Index>(2, myZeroCouponCurve);
        auto myFloatingLeg = std::unique_ptr<Leg>{
                std::make_unique<FloatingLegType>(paymentCalendar, 100, myIndex)
        };

        auto mySwap = Swap(myFixedLeg, myFloatingLeg, myZeroCouponCurve);

        BOOST_TEST_MESSAGE(mySwap()); // todo: calcular caso transparencias
    }

BOOST_AUTO_TEST_SUITE_END()