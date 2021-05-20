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
#include <instruments/deposit.h>

#include <instruments/goalSeeker.h>
#include <market/bootstrapping.h>
#include <instruments/instrumentDescription.h>
#include <instruments/instrumentFactory.h>

using namespace instruments;
using namespace market;
using namespace day_count_fraction;

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
        types::MapDiscountCurveType zeroCurveData{
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

        types::MapDiscountCurveType zeroCurveData{
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
        std::vector<double> correctValues{2.46492, 2.69672, 2.71611, 2.81999};

        // CHECK PAYMENTS DATES
        BOOST_CHECK_EQUAL_COLLECTIONS(paymentsDates.begin(), paymentsDates.end(),
                                      correctDates.begin(), correctDates.end());

        // CHECK PAYS
        for (int i = 0; i < paymentValues.size(); i++) {
            BOOST_TEST(paymentValues.at(i) == correctValues.at(i), boost::test_tools::tolerance(1e-5));
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(zero_coupon)

    BOOST_AUTO_TEST_CASE(test_zeroCoupon_date)
    {
        BOOST_TEST_MESSAGE("Testing Zero Coupon discount curve");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};


        types::MapDiscountCurveType zeroCurveData{
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

    BOOST_AUTO_TEST_CASE(test_bootstrapping)
    {
        BOOST_TEST_MESSAGE("Testing Bootstrapping");

        types::date today = DayCountCalculator::make_date("2016/4/1");

        std::vector<types::date> deposit6MCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3")};

        std::vector<types::date> bond12MCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3")};

        std::vector<types::date> bond18MCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2")};

        std::vector<types::date> bond24MCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::MapDiscountCurveType zeroCurveData{};

        // ZC Curve
        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);

        // Instance Fixed Leg
        typedef FixedLeg<Actual_360> FixedLegType;

        auto myFixedLeg6M = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(deposit6MCalendar, 100, 0.05)
        };

        auto myFixedLeg12M = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(bond12MCalendar, 100, 0.055)
        };

        auto myFixedLeg18M = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(bond18MCalendar, 100, 0.06)
        };

        auto myFixedLeg24M = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(bond24MCalendar, 100, 0.064)
        };

        // Instance Instruments
        // Second leg on the swap instantiation is irrelevant
        auto myDeposit6M = std::shared_ptr<IInstrument>{
                std::make_shared<Deposit>(myFixedLeg6M, myZeroCouponCurve)
        };

        auto mySwap12M = std::shared_ptr<IInstrument>{
                std::make_shared<Swap>(myFixedLeg12M, myFixedLeg12M, myZeroCouponCurve)
        };

        auto mySwap18M = std::shared_ptr<IInstrument>{
                std::make_shared<Swap>(myFixedLeg18M, myFixedLeg12M, myZeroCouponCurve)
        };


        auto mySwap24M = std::shared_ptr<IInstrument>{
                std::make_shared<Swap>(myFixedLeg24M, myFixedLeg12M, myZeroCouponCurve)
        };

        // Bootstrapping
        typedef std::map<types::date, std::shared_ptr<instruments::IInstrument>> InstrumentMapType;
        InstrumentMapType instruments{
                {deposit6MCalendar.back(), myDeposit6M},
                {bond12MCalendar.back(),   mySwap12M},
                {bond18MCalendar.back(),   mySwap18M},
                {bond24MCalendar.back(),   mySwap24M}
        };

        auto myBootstrapping = Bootstrapping();
        auto discountCurveCalibrated = myBootstrapping(instruments);
        std::map<types::date, double> correctDiscountCurve{
                {DayCountCalculator::make_date("2016/10/3"), 0.9749492},
                {DayCountCalculator::make_date("2017/4/3"),  0.9461362},
                {DayCountCalculator::make_date("2017/10/2"), 0.9135292},
                {DayCountCalculator::make_date("2018/04/2"), 0.8793138}
        };

        for (auto it = discountCurveCalibrated.begin(); it != discountCurveCalibrated.end(); ++it) {
            types::date date = it->first;
            double discountFactor = it->second;

            BOOST_TEST(discountFactor == correctDiscountCurve.find(date)->second, boost::test_tools::tolerance(1e-6));
        }
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

        types::MapDiscountCurveType zeroCurveData{
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
        BOOST_TEST(myBond() == 98.36078, boost::test_tools::tolerance(1e-5));
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

        types::MapDiscountCurveType zeroCurveData{
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
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.05)
        };

        // Instance Floating leg
        typedef FloatingLeg<Actual_360> FloatingLegType;
        auto myIndex = std::make_shared<Index>(2, myZeroCouponCurve);
        auto myFloatingLeg = std::unique_ptr<Leg>{
                std::make_unique<FloatingLegType>(paymentCalendar, 100, myIndex)
        };

        auto mySwap = Swap(myFixedLeg, myFloatingLeg, myZeroCouponCurve);

        BOOST_TEST(mySwap() == 0.49573, boost::test_tools::tolerance(1e-5));
    }

    BOOST_AUTO_TEST_CASE(test_deposit) {

        BOOST_TEST_MESSAGE("Testing Deposit instrument");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3")};

        types::MapDiscountCurveType zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474}
        };

        auto myZeroCouponCurve = std::make_shared<ZeroCouponCurve>(zeroCurveData, today);

        // Instance Fixed Leg
        typedef FixedLeg<Actual_360> FixedLegType;
        auto myFixedLeg = std::unique_ptr<Leg>{
                std::make_unique<FixedLegType>(paymentCalendar, 100, 0.05)
        };

        // Instance deposit
        auto myDeposit = Deposit(myFixedLeg, myZeroCouponCurve);

        BOOST_TEST(myDeposit() == 100.1012068, boost::test_tools::tolerance(1e-7));


    }

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(tir)

    BOOST_AUTO_TEST_CASE(test_goal_seeker) {

        BOOST_TEST_MESSAGE("Testing TIR goal seeker");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::MapDiscountCurveType zeroCurveData{
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
        double bondPrice = myBond();

        auto seeker = GoalSeeker(10e-9, 10e-5, 10000);
        auto myFunc = [&](double rate) {
            myZeroCouponCurve->setFixedRate(rate);
            return myBond();
        };

        BOOST_TEST(seeker(myFunc, bondPrice) == 0.067615, boost::test_tools::tolerance(1e-5));
    }


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(factories)

    BOOST_AUTO_TEST_CASE(test_bond_factory) {

        BOOST_TEST_MESSAGE("Testing Bond Factory");

        std::vector<types::date> paymentCalendar{
                day_count_fraction::DayCountCalculator::make_date("2016/4/1"),
                day_count_fraction::DayCountCalculator::make_date("2016/10/3"),
                day_count_fraction::DayCountCalculator::make_date("2017/4/3"),
                day_count_fraction::DayCountCalculator::make_date("2017/10/2"),
                day_count_fraction::DayCountCalculator::make_date("2018/04/2")};

        types::MapDiscountCurveType zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.05},
                {paymentCalendar[2], 0.058},
                {paymentCalendar[3], 0.064},
                {paymentCalendar[4], 0.068},
        };

        instruments::LegDescription fixedLegDescription;
        fixedLegDescription.paymentCalendar = paymentCalendar;
        fixedLegDescription.nominal = 100;
        fixedLegDescription.rate = 0.06;

        instruments::InstrumentDescription instrumentDescription(instruments::InstrumentDescription::Type::bond);
        instrumentDescription.curveData = zeroCurveData;
        instrumentDescription.receiver = fixedLegDescription;

        auto myBond = instruments::InstrumentFactory::instance()(instrumentDescription);

        BOOST_TEST(myBond->operator()() == 98.36078, boost::test_tools::tolerance(1e-5));
    }

    BOOST_AUTO_TEST_CASE(test_deposit_factory) {

        BOOST_TEST_MESSAGE("Testing Deposit Factory");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3")};

        types::MapDiscountCurveType zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474}
        };

        instruments::LegDescription fixedLegDescription;
        fixedLegDescription.paymentCalendar = paymentCalendar;
        fixedLegDescription.nominal = 100;
        fixedLegDescription.rate = 0.05;

        instruments::InstrumentDescription instrumentDescription(instruments::InstrumentDescription::Type::deposit);
        instrumentDescription.curveData = zeroCurveData;
        instrumentDescription.receiver = fixedLegDescription;

        auto myDeposit = instruments::InstrumentFactory::instance()(instrumentDescription);

        BOOST_TEST(myDeposit->operator()() == 100.1012068, boost::test_tools::tolerance(1e-7));

    }

    BOOST_AUTO_TEST_CASE(test_swap_factory) {

        BOOST_TEST_MESSAGE("Testing Swap Factory");

        types::date today = DayCountCalculator::make_date("2016/4/1");
        std::vector<types::date> paymentCalendar{
                today,
                DayCountCalculator::make_date("2016/10/3"),
                DayCountCalculator::make_date("2017/4/3"),
                DayCountCalculator::make_date("2017/10/2"),
                DayCountCalculator::make_date("2018/04/2")};

        types::MapDiscountCurveType zeroCurveData{
                {paymentCalendar[0], 1.},
                {paymentCalendar[1], 0.0474},
                {paymentCalendar[2], 0.05},
                {paymentCalendar[3], 0.051},
                {paymentCalendar[4], 0.052},
        };

        instruments::LegDescription fixedLegDescription;
        fixedLegDescription.paymentCalendar = paymentCalendar;
        fixedLegDescription.nominal = 100;
        fixedLegDescription.rate = 0.05;

        instruments::LegDescription floatingLegDescription;
        floatingLegDescription.paymentCalendar = paymentCalendar;
        floatingLegDescription.nominal = 100;
        floatingLegDescription.indexFrequency = 2;

        instruments::InstrumentDescription instrumentDescription(instruments::InstrumentDescription::Type::swap);
        instrumentDescription.curveData = zeroCurveData;
        instrumentDescription.receiver = fixedLegDescription;
        instrumentDescription.payer = floatingLegDescription;

        auto mySwap = instruments::InstrumentFactory::instance()(instrumentDescription);

        BOOST_TEST(mySwap->operator()() == 0.49573, boost::test_tools::tolerance(1e-5));
    }

BOOST_AUTO_TEST_SUITE_END()