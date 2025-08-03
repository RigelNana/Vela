module;
#include <cassert>
#include <chrono>
export module time_sys;


import constant;
import units;


export namespace vela::time {

    using namespace std::chrono;

    struct CalendarDate {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        double second;

        constexpr auto operator<=>(const CalendarDate&) const = default;
    };

    class JulianDate {
        double jd_;
    public:
        explicit constexpr JulianDate(double jd) : jd_(jd) {}


        JulianDate(const JulianDate&) = default;
        JulianDate(JulianDate&&) = default;

        [[nodiscard]] double value() const {
            return jd_;
        }

        static constexpr JulianDate from_calendar(int year, int month, double day) {
            if (month <= 2) {
                year -= 1;
                month += 12;
            }
            int a = year / 100;
            int b = 2 - a + a / 4;
            double jd_check = 365.25 * (year + 4716) + 30.6001 * (month + 1) + day - 1524.5;
            if (jd_check < 2299161.0) {
                b = 0;
            }
            double jd = static_cast<long long>(365.25 * (year + 4716)) + static_cast<long long>(30.6001 * (month + 1)) + day + b - 1524.5;
            return JulianDate(jd);
        }



        static constexpr JulianDate from_calendar(int year, int month, int day, int hour, int minute, double second) {
            double day_frac = day + (hour / 24.0) + (minute / 1440.0) + (second / 86400.0);
            return from_calendar(year, month, day_frac);
        }
        static constexpr JulianDate from_calendar(const CalendarDate& date) {
            return from_calendar(date.year, date.month, date.day, date.hour, date.minute, date.second);
        }

        [[nodiscard]] constexpr CalendarDate to_calendar() const {
            double jd = jd_;
            int z = static_cast<int>(jd + 0.5);
            double f = jd + 0.5 - z;
            int a = z;
            if (z >= 2299161) {
                int alpha = static_cast<int>((z - 1867216.25) / 36524.25);
                a += 1 + alpha - alpha / 4;
            }
            int b = a + 1524;
            int c = static_cast<int>((b - 122.1) / 365.25);
            int d = static_cast<int>(365.25 * c);
            int e = static_cast<int>((b - d) / 30.6001);
            int day = b - d - static_cast<int>(30.6001 * e) + static_cast<int>(f);
            int month = e < 14 ? e - 1 : e - 13;
            int year = month > 2 ? c - 4716 : c - 4715;

            double hour_frac = f * 24.0;
            int hour = static_cast<int>(hour_frac);
            double minute_frac = (hour_frac - hour) * 60.0;
            int minute = static_cast<int>(minute_frac);
            double second = (minute_frac - minute) * 60.0;

            return CalendarDate{year, month, day, hour, minute, second};
        }
        constexpr auto operator<=>(const JulianDate &) const = default;

    };



     constexpr JulianDate to_julian_date(const system_clock::time_point& tp) {
        const auto secs_since_epoch = std::chrono::duration<double>(tp.time_since_epoch()).count();

        return JulianDate(constant::JD_UNIX_EPOCH + secs_since_epoch / constant::SECONDS_PER_DAY);
    }

    [[nodiscard]] constexpr system_clock::time_point to_time_point(JulianDate jd) {


        const auto secs_since_epoch = duration<double>(
            (jd.value() - constant::JD_UNIX_EPOCH) * constant::SECONDS_PER_DAY
        );

        return system_clock::time_point(
            duration_cast<system_clock::duration>(secs_since_epoch)
        );
    }

    inline JulianDate now_jd() {
        auto now = system_clock::now();
        return to_julian_date(now);
    }

    constexpr units::hours sidereal_time(const JulianDate& jd) {
         double jd_value = jd.value();
         double t = (jd_value - constant::JD_2000) / constant::DAYS_PER_JULIAN_CENTURY;
         // IAU 1982
         auto theta_z = units::degrees( 280.46061837)
             + units::degrees(360.98564736629) * (jd_value - 2451545.0)
             + units::degrees(t * t * (0.000387933 - t / 38710000.0));
         // Normalize to 24 hours
         theta_z = theta_z.normalize();
         return theta_z.to_hours();
     }

}

