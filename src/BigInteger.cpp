#include <iostream>
#include "BigInteger.h"
#include "helpers.h"

namespace BigNum {
    template<typename T>
    T BigInteger::value() {
        if (std::is_unsigned<T>::value && sign < 0)
            throw SignException("Casting negative value to unsigned");

        if (*this > BigInteger::from<T>(std::numeric_limits<T>::max()) ||
            *this < BigInteger::from<T>(std::numeric_limits<T>::min())) {
            auto msg = "BigInteger '" + toString() + "' cannot be represented as type: " + typeid(T).name();
            throw OverflowException(msg.c_str());
        }


        T res = 0;
        size_t pow = 1;
        for (auto digit : digits) {
            res += digit * pow;
            pow *= 10;
        }
        res *= sign;
        return res;
    }

    std::string BigInteger::toString() const {
        std::stringstream res;
        if (sign < 0)
            res << '-';
        if (digits.empty())
            return "0";

        for (auto it = digits.rbegin(); it != digits.rend(); it++) {
            res << std::to_string(*it);
        }
        return res.str();
    }

    BigInteger::BigInteger(int64_t val) : BigInteger(std::to_string(val)) {}

    BigInteger::BigInteger(std::string str) {
        trim(str);
        if (str.empty())
            throw std::invalid_argument("Cannot parse empty string");

        if (str[0] == '-') {
            sign = -1;
            str = std::string(str.begin() + 1, str.end());
        }
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            if (!std::isdigit(*it))
                throw std::invalid_argument("Cannot parse string as number");

            digits.push_back(*it - '0'); // ascii value to actual number
        }

    }

    bool BigInteger::operator>(const BigInteger &oth) const {
        if (sign > 0 && oth.sign < 0)return true;
        if (sign < 0 && oth.sign > 0)return false;


        if (oth.digits.size() != digits.size())
            return (digits.size() > oth.digits.size()) != (sign < 0);
        for (int64_t i = digits.size() - 1; i >= 0; i--) {
            if (digits.at(i) != oth.digits.at(i)) {
                return (digits.at(i) > oth.digits.at(i)) != (sign < 0);
            }
        }

        return false; // equal
    }

    bool BigInteger::operator<(const BigInteger &oth) const {
        return !(*this >= oth);
    }

    bool BigInteger::operator>=(const BigInteger &oth) const {
        return *this > oth || *this == oth;
    }

    bool BigInteger::operator==(const BigInteger &oth) const {
        return sign == oth.sign && digits == oth.digits;
    }

    bool BigInteger::operator!=(const BigInteger &oth) const {
        return !(*this == oth);
    }

    bool BigInteger::operator<=(const BigInteger &oth) const {
        return *this < oth || *this == oth;
    }

    template<typename T>
    BigInteger BigInteger::from(T val) {
        return BigInteger(std::to_string(val));
    }

    bool BigInteger::operator>(int64_t val) const {
        return *this > BigInteger(val);
    }

    bool BigInteger::operator<(int64_t val) const {
        return *this < BigInteger(val);
    }

    bool BigInteger::operator>=(int64_t val) const {
        return *this >= BigInteger(val);
    }

    bool BigInteger::operator<=(int64_t val) const {
        return *this <= BigInteger(val);
    }

    bool BigInteger::operator!=(int64_t val) const {
        return *this != BigInteger(val);
    }

    bool BigInteger::operator==(int64_t val) const {
        return *this == BigInteger(val);
    }

    BigInteger BigInteger::operator+(const BigInteger &oth) const {
        if (sign != oth.sign) {
            if (sign < 0) {
                return oth - this->abs();
            }
            return *this - oth.abs();
        }
        BigInteger res;
        int rest = 0;
        for (size_t i = 0; i < std::max(digits.size(), oth.digits.size()); i++) {
            res.digits.push_back(rest);
            if (i < digits.size())
                res.digits.back() += digits.at(i);
            if (i < oth.digits.size())
                res.digits.back() += oth.digits.at(i);
            rest = res.digits.back() / 10;
            res.digits.back() %= 10;
        }

        if (rest)
            res.digits.push_back(rest);
        res.sign = sign;
        res.trimZeros();
        return res;
    }

    BigInteger BigInteger::operator*(const BigInteger &oth) const {
        BigInteger res;
        if (sign != oth.sign)
            return this->abs() * oth.abs();

        for (size_t i = 0; i < oth.digits.size(); i++) {
            BigInteger part;
            auto mul = this->abs();
            for (int _ = 0; _ < oth.digits.at(i); _++)
                part += mul;
            part.multiplyByFactorOf10(i);
            res += part;
        }

        return res;
    }

    BigInteger BigInteger::operator-(const BigInteger &oth) const {
        if (sign != oth.sign) {
            if (sign < 0) {
                return -(oth + this->abs());
            }
            return *this + oth.abs();
        }
        if (sign < 0 && oth.sign < 0) {
            return oth.abs() - this->abs();
        }
        if (oth.abs() > *this)
            return -(oth.abs() - *this);
        BigInteger res;
        int rest = 0;
        for (size_t i = 0; i < std::max(digits.size(), oth.digits.size()); i++) {
            res.digits.push_back(rest);
            rest = 0;
            if (i < digits.size())
                res.digits.back() += digits.at(i);
            if (i < oth.digits.size())
                res.digits.back() -= oth.digits.at(i);
            if (res.digits.back() < 0) {
                rest = -1;
                res.digits.back() = 10 + res.digits.back();
            }
        }

        if (rest)
            res.sign = -1;
        res.trimZeros();
        return res;
    }

    BigInteger BigInteger::operator/(const BigInteger &oth) const {
        if (oth == 0)
            throw std::invalid_argument("Division by zero");
        if (this->abs() < oth.abs())
            return {};

        int factor = 0;
        BigInteger oth_copy = oth.abs();
        auto this_abs = abs();
        while (this_abs > oth_copy) {
            oth_copy.multiplyByFactorOf10(1);
            factor++;
        }

        BigInteger lower{1};
        BigInteger upper{1};
        lower.multiplyByFactorOf10(factor - 1);
        upper.multiplyByFactorOf10(factor);
        while (upper - lower > 1) {
            auto middle = upper + lower;
            middle.divideBy2();
            if (middle * oth.abs() < this_abs) {
                lower = middle;
            } else {
                upper = middle;
            }
        }
        lower.sign = sign * oth.sign;
        return lower;

    }

    BigInteger BigInteger::operator+(int64_t val) const {
        return *this + BigInteger(val);
    }

    BigInteger BigInteger::operator*(int64_t val) const {
        return *this * BigInteger(val);
    }

    BigInteger BigInteger::operator-(int64_t val) const {
        return *this - BigInteger(val);
    }


    BigInteger BigInteger::operator/(int64_t val) const {
        return *this / BigInteger(val);
    }

    BigInteger BigInteger::abs() const {
        BigInteger res;
        res.digits = digits;
        return res;
    }

    BigInteger &BigInteger::operator+=(const BigInteger &oth) {
        *this = *this + oth;
        return *this;
    }

    BigInteger &BigInteger::operator-=(const BigInteger &oth) {
        *this = *this - oth;
        return *this;
    }

    BigInteger &BigInteger::operator*=(const BigInteger &oth) {
        *this = *this * oth;
        return *this;
    }

    BigInteger &BigInteger::operator/=(const BigInteger &oth) {
        *this = *this / oth;
        return *this;
    }

    BigInteger &BigInteger::operator+=(int64_t val) {
        *this = *this + val;
        return *this;
    }

    BigInteger &BigInteger::operator-=(int64_t val) {
        *this = *this - val;
        return *this;
    }

    BigInteger &BigInteger::operator*=(int64_t val) {
        *this = *this * val;
        return *this;
    }

    BigInteger &BigInteger::operator/=(int64_t val) {
        *this = *this / val;
        return *this;
    }

    BigInteger operator+(int64_t val, const BigInteger &bi) {
        return BigInteger(val) + bi;
    }

    BigInteger operator-(int64_t val, const BigInteger &bi) {
        return BigInteger(val) - bi;
    }

    BigInteger operator*(int64_t val, const BigInteger &bi) {
        return BigInteger(val) * bi;
    }

    BigInteger operator/(int64_t val, const BigInteger &bi) {
        return BigInteger(val) / bi;
    }

    double BigInteger::realDivide(uint64_t val) const {
        return realDivide(BigInteger(val));
    }

    double BigInteger::realDivide(const BigInteger &oth) const {
        if (oth == 0)
            throw std::invalid_argument("Division by zero");
        return toDouble() / oth.toDouble();
    }

    BigInteger BigInteger::operator-() const {
        BigInteger res = *this;
        res.sign = sign > 0 ? -1 : 1;
        return res;
    }

    void BigInteger::trimZeros() {
        for (auto it = digits.rbegin(); it != digits.rend(); it++) {
            if (*it != 0) {
                break;
            }
            digits.pop_back();
        }
    }

    // insert leading zeros to vector of digits
    void BigInteger::multiplyByFactorOf10(int factor) {
        std::vector<int8_t> zeros(factor, 0);
        digits.insert(digits.begin(), zeros.begin(), zeros.end());
        trimZeros();
    }

    void BigInteger::divideBy2() {
        for (auto it = digits.rbegin(); it != digits.rend(); it++) {
            if (*it % 2 == 1) {
                auto next = it;
                next++;
                if (next != digits.rend())
                    *next += 10;
            }
            *it /= 2;
        }
        trimZeros();
    }

    BigInteger BigInteger::operator%(const BigInteger &oth) const {
        BigInteger res = *this - (oth * (*this / oth));
        if (sign > 0)
            return res;
        return -(res.abs() % oth);
    }

    BigInteger BigInteger::operator%(int64_t val) const {
        return *this % BigInteger(val);
    }

    BigInteger &BigInteger::operator%=(int64_t val) {
        *this = *this % BigInteger(val);
        return *this;
    }

    BigInteger &BigInteger::operator%=(const BigInteger &oth) {
        *this = *this % oth;
        return *this;
    }

    BigInteger operator%(int64_t val, const BigInteger &bi) {
        return BigInteger(val) % bi;
    }

    double BigInteger::toDouble() const {
        double res = 0;
        double tens = 1;
        for (auto d : digits) {
            res += (d * tens);
            tens *= 10;
        }
        return sign * res;
    }


    template BigInteger BigInteger::from(uint64_t val);

    template BigInteger BigInteger::from(int64_t val);

    template BigInteger BigInteger::from(uint32_t val);

    template BigInteger BigInteger::from(int32_t val);

    template BigInteger BigInteger::from(uint16_t val);

    template BigInteger BigInteger::from(int16_t val);

    template BigInteger BigInteger::from(uint8_t val);

    template BigInteger BigInteger::from(int8_t val);


    template uint64_t BigInteger::value<uint64_t>();

    template int64_t BigInteger::value<int64_t>();

    template int32_t BigInteger::value<int32_t>();

    template uint32_t BigInteger::value<uint32_t>();

    template uint16_t BigInteger::value<uint16_t>();

    template int16_t BigInteger::value<int16_t>();

    template int8_t BigInteger::value<int8_t>();

    template uint8_t BigInteger::value<uint8_t>();


    BigInteger::OverflowException::OverflowException(char const *const message) noexcept : invalid_argument(message) {}

    char const *BigInteger::OverflowException::what() const noexcept {
        return logic_error::what();
    }

    BigInteger::SignException::SignException(char const *const message) noexcept : invalid_argument(message) {}

    char const *BigInteger::SignException::what() const noexcept {
        return logic_error::what();
    }
}
