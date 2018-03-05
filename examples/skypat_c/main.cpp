//===- main.cpp -----------------------------------------------------------===//
//
//                               The SkyPat Team
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// If you're like us, you'd like to look at some SkyPat sample code. The
// examples/ folder has a numebr of well-commented samples show you how to
// use SkyPat features.
//
// This sample shows how to write a simple performance test for a function,
// using SkyPat framework.
//
//===----------------------------------------------------------------------===//

// Step 1. Include necessary header files such that the stuff your test logic
// needs is declared.
#include "skypat/skypat.h"
#include "my_case.h"

// Step 2. Use the macro to define your performance test.
//
// SKYPAT macro has two parameters: the case name and the test name.
// After using the macro, you must define your test logic between a pair of
// braces.
//
// In SkyPat, test logics are grouped into `cases`. Every case has a bunch of
// `tests`. This is how we organize the test codes. You should put logically
// related tests into the same test case.
//
// Every test contains two parts: performance test and unit-test.
// PERFORM macro is used to measure the performance of code within a test;
// ASSERT macro compares the actual value to an expected value by using the
// equals operator. skypat remembers the performance number of a test only if
// all unit-tests in the test are passed.
//
// The names of cases and tests should be valid C++ identifiers.
//
// SkyPat does NOT guarantee that every tests you define is run exactly once.
// In fact, SkyPat runs a test repeatedly for getting precise performance
// numbers. Therefore, you should guarantee that in all runs the initial value
// of a variable of a test is fixed.

class MyCase : public skypat::Test
{
public:
    // create a turret for all test.
    void SetUp() {
        m_turret = TurretFactory::CreateTurret();
    }
    // clear up the memory.
    void TearDown() {
        TurretFactory::DestoryTurret(m_turret);
    }
    Turret * getTurret() { return m_turret; };
private:
    Turret * m_turret;
};

// test the default status of turret
SKYPAT_C(MyCase, test_turret_is_turn_by_default)
{
    Turret * turret = this->getTurret();
    EXPECT_TRUE( true==turret->isOn() );
}

// test the turn on turn of functions of turret
SKYPAT_C(MyCase, test_turret_can_be_turn_off)
{
    Turret * turret = this->getTurret();
    turret->turnOff();
    EXPECT_TRUE( false==turret->isOn() );
}

// test the turn on turn of functions of turret
SKYPAT_C(MyCase, test_turret_can_fire_iff_turn_on)
{
    Turret * turret = this->getTurret();
    turret->turnOff();
    EXPECT_TRUE( false==turret->fire() );

    turret->turnOn();
    EXPECT_TRUE( true==turret->fire() );
}

// Performance Test
SKYPAT_C(MyCase, performance_turret_fire)
{
    // Make sure the turret is On.
    Turret * turret = this->getTurret();
    turret->turnOn();
    ASSERT_TRUE( turret->isOn() );

    // Start performance test
    PERFORM(skypat::CONTEXT_SWITCHES) {
       turret->fire();
    }
}

// Step 3. Call RunAll() in main().
//
// This runs all the tests you've defined, prints the result and
// return 0 if succeed, or 1 otherwise.
//
// Did you notice that we didn't register any tests? The RunAll()
// function magically knows about all the test we defined. Isn't it
// convenient?
int main(int argc, char* argv[])
{
  skypat::Test::Initialize(argc, argv);
  skypat::Test::RunAll();
}
