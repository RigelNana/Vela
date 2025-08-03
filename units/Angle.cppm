//
// Created by RigelShrimp on 2025/8/2.
//

module;
#include <concepts>
#include <cmath>
#include <algorithm>
#include <format>
#include <compare>
export module units;



export namespace vela::units {
    namespace details {
        struct radians_t {};
        struct degrees_t {};
        struct hours_t {};
    } // namespace details


    template <typename UnitTag>
    class Quantity {
        double value_;
    public:
        constexpr explicit Quantity(double val) : value_(val){}
        Quantity(const Quantity&) = default;
        Quantity(Quantity&&) = default;

        Quantity& operator=(const Quantity&) = default;
        Quantity& operator=(Quantity&&) = default;

        [[nodiscard]] constexpr double value() const { return value_; }
        constexpr Quantity operator+(const Quantity& other) const {
            return Quantity(value_ + other.value_);
        }
        constexpr Quantity operator-(const Quantity& other) const {
            return Quantity(value_ - other.value_);
        }
        constexpr Quantity operator*(double scalar) const {
            return Quantity(value_ * scalar);
        }
        constexpr Quantity operator/(double scalar) const {
            return Quantity(value_ / scalar);
        }
        constexpr Quantity operator-() const {
            return Quantity(-value_);
        }
        constexpr auto operator<=>(const Quantity& other) const = default;


        [[nodiscard]] constexpr auto to_radians() const {
            if constexpr (std::is_same_v<UnitTag, details::radians_t>) {
                return *this;
            } else if constexpr (std::is_same_v<UnitTag, details::degrees_t>) {
                return Quantity<details::radians_t>(value_ * std::numbers::pi / 180.0);
            } else if constexpr (std::is_same_v<UnitTag, details::hours_t>) {
                return Quantity<details::radians_t>(value_ * 15.0 * std::numbers::pi / 180.0);
            }
        }
        [[nodiscard]] constexpr auto to_degrees() const {
            if constexpr (std::is_same_v<UnitTag, details::degrees_t>) {
                return *this;
            } else if constexpr (std::is_same_v<UnitTag, details::radians_t>) {
                return Quantity<details::degrees_t>(value_ * 180.0 / std::numbers::pi);
            } else if constexpr (std::is_same_v<UnitTag, details::hours_t>) {
                return Quantity<details::degrees_t>(value_ * 15.0);
            }
        }
        [[nodiscard]] constexpr auto to_hours() const {
            if constexpr (std::is_same_v<UnitTag, details::hours_t>) {
                return *this;
            } else if constexpr (std::is_same_v<UnitTag, details::radians_t>) {
                return Quantity<details::hours_t>(value_ / (15.0 * std::numbers::pi / 180.0));
            } else if constexpr (std::is_same_v<UnitTag, details::degrees_t>) {
                return Quantity<details::hours_t>(value_ / 15.0);
            }
        }
        [[nodiscard]] constexpr auto normalize() const {
            if constexpr(std::is_same_v<UnitTag, details::degrees_t>) {
                constexpr double cycle = 360.0;
                double val = std::fmod(value_, cycle);
                return Quantity(val >= 0 ? val : val + cycle);
            } else if constexpr(std::is_same_v<UnitTag, details::radians_t>) {
                constexpr double cycle = 2 * std::numbers::pi;
                double val = std::fmod(value_, cycle);
                return Quantity(val >= 0 ? val : val + cycle);
            } else if constexpr(std::is_same_v<UnitTag, details::hours_t>) {
                constexpr double cycle = 24.0;
                double val = std::fmod(value_, cycle);
                return Quantity(val >= 0 ? val : val + cycle);
            } else {
                static_assert(false, "Unsupported unit type for normalization");
            }
        }


        [[nodiscard]] constexpr Quantity normalize_symmetric() const {
            Quantity normalized = this->normalize(); // 先规范化到 [0, cycle)
            double cycle;
            if constexpr (std::is_same_v<UnitTag, details::degrees_t>) {
                cycle = 360.0;
            } else if constexpr (std::is_same_v<UnitTag, details::radians_t>) {
                cycle = std::numbers::pi * 2.0;
            } else if constexpr (std::is_same_v<UnitTag, details::hours_t>) {
                cycle = 24.0;
            } else {
                return *this;
            }

            if (normalized.value() >= cycle / 2.0) {
                return Quantity(normalized.value() - cycle);
            }
            return normalized;
        }

        [[nodiscard]] constexpr auto clamp(const Quantity& min, const Quantity& max) const {
            return Quantity(std::clamp(value_, min.value(), max.value()));
        }

    };


    using radians = Quantity<details::radians_t>;
    using degrees = Quantity<details::degrees_t>;
    using hours = Quantity<details::hours_t>;
    template <typename T>
    concept Angle = std::is_same_v<T, radians> ||
                    std::is_same_v<T, degrees> ||
                    std::is_same_v<T, hours>;
    namespace literals {
        constexpr radians operator""_rad(long double val) {
            return radians(static_cast<double>(val));
        }
        constexpr degrees operator""_deg(long double val) {
            return degrees(static_cast<double>(val));
        }
        constexpr hours operator""_hr(long double val) {
            return hours(static_cast<double>(val));
        }
    }


    constexpr auto make_degrees(long long degrees, long long minutes, double seconds) {
        return units::degrees(degrees + (minutes / 60.0) + (seconds / 3600.0));
    }


    constexpr auto make_hours(long long hours, long long minutes, double seconds) {
        return units::hours(hours + (minutes / 60.0) + (seconds / 3600.0));
    }




}








