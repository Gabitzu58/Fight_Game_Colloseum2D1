#pragma once
#include <stdexcept>
#include <ostream>

template <typename T>
class Stat {
private:
    T value;
    T maxValue;

public:
    void reset(T newMax) {
        if (newMax <= 0)
            throw std::invalid_argument("Invalid max value");
        maxValue = newMax;
        value = newMax;
    }

    Stat(T value, T maxValue) : value(value), maxValue(maxValue) {
        if (value < 0 || maxValue <= 0 || value > maxValue)
            throw std::invalid_argument("Stat values invalid");
    }

    void increase(T amount) {
        if (amount < 0)
            throw std::invalid_argument("Increase must be positive");
        value = std::min(value + amount, maxValue);
    }

    void decrease(T amount) {
        if (amount < 0)
            throw std::invalid_argument("Decrease must be positive");
        value = std::max(static_cast<T>(0), value - amount);
    }

    T get() const {
        return value;
    }

    T getMax() const {
        return maxValue;
    }

    friend std::ostream& operator<<(std::ostream& os, const Stat<T>& stat) {
        os << stat.value << "/" << stat.maxValue;
        return os;
    }
};
