#include <print>
#include <format>
#include <thread>

import units;
import time_sys;
import coord;
import transform;
using namespace vela::units::literals;

int main() {
    auto jd = vela::time::JulianDate::from_calendar(1987, 4, 10, 19, 21, 0.0);
    auto sidereal = vela::time::sidereal_time(jd);
    auto geo = vela::coord::geographical(vela::units::make_degrees(-77, 3, 56),
                                         vela::units::make_degrees(38, 55, 17.0));
    auto venus = vela::coord::equatorial(vela::units::make_hours(23, 9, 16.641), vela::units::make_degrees(-6, 43, 11.61));
    auto horizontal = vela::coord::trans::to_horizontal(venus, geo, jd);
    std::println("Horizontal Coordinates: Azimuth = {}, Altitude = {}",
                 horizontal.azimuth().value(), horizontal.altitude().value());
    std::println("Sidereal Time: {}", sidereal.value());
    auto hour_angle = vela::coord::hour_angle(venus, geo, jd);
    std::println("Hour Angle: {}", hour_angle.to_degrees().value());
    auto eq = vela::coord::trans::to_equatorial(horizontal, geo, jd);
    std::println("Equatorial Coordinates: RA = {}, Dec = {}",
                 eq.right_ascension().value(), eq.declination().value());
}