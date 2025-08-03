module;
export module coord;
import units;
import time_sys;

using namespace vela::units::literals;
export namespace vela::coord {
    namespace details {
        struct horizontal_t {};
        struct equatorial_t {};
        struct ecliptical_t {};
        struct galactical_t {};
        struct geographical_t {};
    }

    template <typename System>
    class Coord;

    template <>
    class Coord<details::horizontal_t> {
        units::degrees azimuth_;
        units::degrees altitude_;
    public:
        Coord(const units::degrees& azimuth, const units::degrees& altitude)
            : azimuth_(azimuth.normalize()), altitude_(altitude.clamp(-90.0_deg, 90.0_deg)) {}

        Coord(const Coord&) = default;
        Coord(Coord&&) = default;

        [[nodiscard]] units::degrees azimuth() const { return azimuth_; }
        [[nodiscard]] units::degrees altitude() const { return altitude_; }
    };

    template <>
    class Coord<details::equatorial_t> {
        units::hours right_ascension_;
        units::degrees declination_;
    public:
        Coord(const units::hours& right_ascension, const units::degrees& declination)
            : right_ascension_(right_ascension.normalize()), declination_(declination.clamp(-90.0_deg, 90.0_deg)) {}

        Coord(const Coord&) = default;
        Coord(Coord&&) = default;

        [[nodiscard]] units::hours right_ascension() const { return right_ascension_; }
        [[nodiscard]] units::degrees declination() const { return declination_; }


    };

    template <>
    class Coord<details::ecliptical_t> {
        units::degrees longitude_;
        units::degrees latitude_;
    public:
        Coord(const units::degrees& longitude, const units::degrees& latitude)
            : longitude_(longitude.normalize()), latitude_(latitude.clamp(-90.0_deg, 90.0_deg)) {}

        Coord(const Coord&) = default;
        Coord(Coord&&) = default;

        [[nodiscard]] units::degrees longitude() const { return longitude_; }
        [[nodiscard]] units::degrees latitude() const { return latitude_; }
    };

    template <>
    class Coord<details::galactical_t> {
        units::degrees galactic_longitude_;
        units::degrees galactic_latitude_;
    public:
        Coord(const units::degrees& galactic_longitude, const units::degrees& galactic_latitude)
            : galactic_longitude_(galactic_longitude.normalize()), galactic_latitude_(galactic_latitude.clamp(-90.0_deg, 90.0_deg)) {}

        Coord(const Coord&) = default;
        Coord(Coord&&) = default;

        [[nodiscard]] units::degrees galactic_longitude() const { return galactic_longitude_; }
        [[nodiscard]] units::degrees galactic_latitude() const { return galactic_latitude_; }
    };

    template <>
    class Coord<details::geographical_t> {
        units::degrees longitude_;
        units::degrees latitude_;

    public:
        Coord(const units::degrees& longitude, const units::degrees& latitude)
            : longitude_(longitude.normalize()), latitude_(latitude.clamp(-90.0_deg, 90.0_deg)) {}

        Coord(const Coord&) = default;
        Coord(Coord&&) = default;

        [[nodiscard]] units::degrees longitude() const { return longitude_; }
        [[nodiscard]] units::degrees latitude() const { return latitude_; }
    };






    using horizontal = Coord<details::horizontal_t>;
    using equatorial = Coord<details::equatorial_t>;
    using ecliptical = Coord<details::ecliptical_t>;
    using galactical = Coord<details::galactical_t>;
    using geographical = Coord<details::geographical_t>;

    constexpr units::hours local_sidereal_time(const time::JulianDate & jd, const coord::geographical& geo) {
        return (time::sidereal_time(jd) + geo.longitude().to_hours()).normalize();
    }
    inline units::hours hour_angle(const equatorial& eq, const geographical& geo, const time::JulianDate& jd) {
        auto lst = local_sidereal_time(jd, geo);
        return (lst - eq.right_ascension()).normalize();
    }



}
