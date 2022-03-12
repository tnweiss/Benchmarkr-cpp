//
// Created by tnwei on 3/1/2022.
//

#include "benchmarkr-dev/benchmarkr.h"

using namespace std::chrono_literals;

CONFIGURE_SOFTWARE_VERSION("1.2.3")

static void set_up (std::map<std::string, void*>& data) {
  std::this_thread::sleep_for(4000ms);
}

static void tear_down (std::map<std::string, void*>& data) {
  std::this_thread::sleep_for(4000ms);
}

static std::mt19937& random_generator() {
  // random number generator used to shuffle tests
  static auto rd = std::random_device("");
  static std::mt19937 g(rd());
  return g;
}

static double multiplier () {
  static double m = 1.00;
  m -= .0003;
  return m;
}

static std::chrono::milliseconds random_ms(long low, long high) {
  std::uniform_int_distribution<long> distrib(low, high);
  long t = distrib(random_generator());
  return std::chrono::milliseconds (static_cast<long>(t * multiplier()));
}

BENCHMARK( GetUserTest, (
    prop_LowerBound_ms( 90 ),
    prop_UpperBound_ms(120),
    prop_("tag", "user"),
    prop_Description( "Get User Details From the Database")),
    ( hook_Setup(set_up), hook_Teardown(tear_down) ) ) {
  std::this_thread::sleep_for(random_ms(120, 130));
}

BENCHMARK( CreateUserTest, (
    prop_LowerBound_ms( 30 ),
        prop_UpperBound_ms(110),
        prop_Description( "Insert user into the database")),
           ( hook_Setup(set_up), hook_Teardown(tear_down) ) ) {
  std::this_thread::sleep_for(random_ms(110, 120));
}

BENCHMARK( UpdateUserTest, (
    prop_LowerBound_ms( 60 ),
        prop_UpperBound_ms(110),
        prop_Description( "Test updating a user in the database")),
           ( hook_Setup(set_up), hook_Teardown(tear_down) ) ) {
  std::this_thread::sleep_for(random_ms(100, 105));
}

BENCHMARK( DeleteUserTest, (
    prop_LowerBound_ms( 50 ),
        prop_UpperBound_ms(80))) {
  std::this_thread::sleep_for(random_ms(80, 90));
}

BENCHMARK( QueryUsersTest, (
    prop_LowerBound_ms( 50 ),
        prop_UpperBound_ms(80))) {
  std::this_thread::sleep_for(random_ms(75, 85));
}

BENCHMARK( FilterRowTest, (
    prop_LowerBound_ms( 110 ),
        prop_UpperBound_ms(130))) {
  std::this_thread::sleep_for(random_ms(130, 140));
}

BENCHMARK( EnrichDataTest, (
    prop_LowerBound_ms( 50 ),
        prop_UpperBound_ms(60))) {
  std::this_thread::sleep_for(random_ms(45, 55));
}

BENCHMARK( NormalizeDataTest, (
    prop_LowerBound_ms( 110 ),
        prop_UpperBound_ms(140))) {
  std::this_thread::sleep_for(random_ms(120, 130));
}

BENCHMARK( SquareRootTest , (prop_LowerBound_us(110))) {
  std::this_thread::sleep_for(random_ms(100, 120));
}

BENCHMARK( AddTwoIntegers, (prop_UpperBound_ms( 20 ))) {
  std::this_thread::sleep_for(random_ms(20, 25));
}

BENCHMARK( SquareTest, (prop_("power", 90)) ) {
  std::this_thread::sleep_for(random_ms(10, 20));
};
