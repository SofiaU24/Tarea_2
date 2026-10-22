#pragma once
#include <chrono>

// Cronómetro simple para medir el tiempo de generación (métrica del informe).
class Timer {
    using clock = std::chrono::steady_clock;

public:
    Timer() : m_start(clock::now()) {}
    double elapsedMs() const {
        return std::chrono::duration<double, std::milli>(clock::now() - m_start).count();
    }

private:
    clock::time_point m_start;
};
