//
// Created by rigel on 25-8-4.
//
module;
#include <cmath>
export module utils;
import units;
import coord;
import time_sys;

export namespace vela::utils {
    inline units::radians parallactic_angle(const coord::geographical& geo, const coord::equatorial& eq, const vela::time::JulianDate& jd) {
        auto H = vela::coord::hour_angle(eq, geo, jd);
        auto sind = std::sin(eq.declination().to_radians().value());
        auto cosd = std::cos(eq.declination().to_radians().value());
        auto sin_h = std::sin(H.to_radians().value());
        auto cos_h = std::cos(H.to_radians().value());
        auto angle = units::radians(std::atan2(sin_h, cos_h * std::tan(geo.latitude().to_radians().value()) -
                                    sind * cos_h));
        return angle;
    }
}