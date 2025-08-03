#include <print>
#include <format>
#include <thread>

import units;
import time_sys;
import coord;
using namespace vela::units::literals;

int main() {
    auto sidereal = vela::time::sidereal_time(vela::time::now_jd());
    std::println("Sidereal Time: {}", sidereal.value());


}