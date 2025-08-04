module;
#include <cmath>
#include <numbers>
export module transform;
import time_sys;
import coord;
import units;

export namespace vela::coord::trans {
    inline horizontal to_horizontal(const equatorial& eq, const geographical& geo, const time::JulianDate& jd) {
        auto hour_angle = vela::coord::hour_angle(eq, geo, jd);
        auto sin_h = std::sin(hour_angle.to_radians().value());
        auto cos_h = std::cos(hour_angle.to_radians().value());
        auto sin_dec = std::sin(eq.declination().to_radians().value());
        auto cos_dec = std::cos(eq.declination().to_radians().value());
        auto sin_lat = std::sin(geo.latitude().to_radians().value());
        auto cos_lat = std::cos(geo.latitude().to_radians().value());
        auto azimuth = units::radians(std::atan2(sin_h, cos_h * sin_lat - (sin_dec / cos_dec) * cos_lat) +
                                      std::numbers::pi);
        auto altitude = units::radians(std::asin(sin_lat * sin_dec + cos_lat * cos_dec * cos_h));
        return horizontal(azimuth.to_degrees(), altitude.to_degrees());
    }
    inline ecliptical to_ecliptical(const equatorial& eq, const double eps = 0.40909280228) {
        auto sina = std::sin(eq.right_ascension().to_radians().value());
        auto cosa = std::cos(eq.right_ascension().to_radians().value());
        auto sind = std::sin(eq.declination().to_radians().value());
        auto cosd = std::cos(eq.declination().to_radians().value());
        auto lambda = units::radians(std::atan2(sina * std::cos(eps) + (sind / cosd) * std::sin(eps), cosa));
        auto beta = units::radians(std::asin(sind * std::cos(eps) - cosd * std::sin(eps) * sina));
        return ecliptical(lambda.to_degrees(), beta.to_degrees());
    }

    inline equatorial to_equatorial(const ecliptical& ec, const double eps = 0.40909280228) {
        auto sinl = std::sin(ec.longitude().to_radians().value());
        auto cosl = std::cos(ec.longitude().to_radians().value());
        auto sinb = std::sin(ec.latitude().to_radians().value());
        auto cosb = std::cos(ec.latitude().to_radians().value());
        auto ra = units::radians(std::atan2(sinl * std::cos(eps) - (sinb / cosb) * std::sin(eps), cosl));
        auto dec = units::radians(std::asin(sinb * std::cos(eps) + cosb * std::sin(eps) * sinl));
        return equatorial(ra.to_hours(), dec.to_degrees());
    }

    inline equatorial to_equatorial(const horizontal& hor, const geographical& geo, const time::JulianDate& jd) {
        auto sin_az = std::sin(hor.azimuth().to_radians().value());
        auto cos_az = std::cos(hor.azimuth().to_radians().value());
        auto sin_alt = std::sin(hor.altitude().to_radians().value());
        auto cos_alt = std::cos(hor.altitude().to_radians().value());
        auto H = units::radians(std::atan2(sin_az, cos_az * std::sin(geo.latitude().to_radians().value()) -
                                      (sin_alt / cos_alt) * std::cos(geo.latitude().to_radians().value())) +
                                      std::numbers::pi);
        auto sidereal = vela::time::sidereal_time(jd);
        auto ra = units::radians(sidereal.to_radians() - H.to_radians()
                                  + geo.longitude().to_radians());
        auto dec = units::radians(std::asin(std::sin(hor.altitude().to_radians().value()) *
                                      std::sin(geo.latitude().to_radians().value()) +
                                      std::cos(hor.altitude().to_radians().value()) *
                                      std::cos(geo.latitude().to_radians().value()) *
                                      std::cos(hor.azimuth().to_radians().value())));
        return equatorial(ra.to_hours(), dec.to_degrees());
    }

}