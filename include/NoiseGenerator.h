#pragma once
#include <random>
#include <cstdint>
#include <array>
#include "Grid.h" //matriz 2d

class NoiseGenerator {
public:
    // Constructor: permite pasar una semilla fija o generar una aleatoria por defecto
    explicit NoiseGenerator(uint32_t seed = 0);

    // Cambiar la semilla del generador aleatorio
    void setSeed(uint32_t seed);

    // Perlin Noise
    Grid<float> perlin(int width, int height, float scale);

    //Perlin fractal: suma varias capas de perlin
    Grid<float> fractal(int width, int height, float scale, int octaves, float persistence, float lacunarity);

private:
    std::mt19937 m_rng;
    std::array<int, 512> m_p;

    void initPermutation();
    static float fade(float t); //curva de suavizado 
    static float lerp(float t, float a, float b); // interpolación lineal
    static float grad(int hash, float x, float y); // gradiante
    float evalPerlin(float x, float y) const;
};