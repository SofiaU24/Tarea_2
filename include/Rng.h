#pragma once
#include <cstdint>
#include <random>

// Generador aleatorio con semilla. Úsalo en vez de rand() para que
// la misma semilla produzca SIEMPRE el mismo mundo (reproducible).
class Rng {
public:
    explicit Rng(uint32_t seed = 0) : m_engine(seed) {}

    void seed(uint32_t s) { m_engine.seed(s); }

    float next01() {                                   // float en [0, 1)
        return std::uniform_real_distribution<float>(0.0f, 1.0f)(m_engine);
    }
    int range(int lo, int hi) {                        // entero en [lo, hi] (ambos incluidos)
        return std::uniform_int_distribution<int>(lo, hi)(m_engine);
    }
    bool chance(float p) { return next01() < p; }      // true con probabilidad p (0..1)

    std::mt19937& engine() { return m_engine; }

private:
    std::mt19937 m_engine;
};

// Deriva una semilla distinta por cada etapa a partir de la semilla principal.
// Así, mover un slider de los árboles no cambia el terreno ni las cuevas.
inline uint32_t deriveSeed(int baseSeed, uint32_t salt) {
    return static_cast<uint32_t>(baseSeed) * 2654435761u + salt * 40503u + 12345u;
}
