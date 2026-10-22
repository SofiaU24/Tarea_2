#pragma once
#include <cstdint>

// Tipos de celda que puede tener un nivel. Agrega los que necesites
// (y su color en render/LevelTexture.cpp).
enum class Tile : uint8_t {
    // --- Pueblo ---
    Water,      // agua (Perlin bajo el nivel del mar)
    Sand,       // orilla
    Grass,      // pasto
    Rock,       // colinas / roca (Perlin alto)
    Road,       // calles (pasillos BSP)
    House,      // casas (habitaciones BSP)
    Tree,       // árboles (L-System)
    // --- Cuevas ---
    Wall,       // roca sólida
    Floor,      // suelo de cueva (CA)
    Ore,        // mineral (Perlin)
    CaveWater,  // agua subterránea (Perlin)
    Root,       // raíces (L-System)
    // --- Ambos ---
    Entrance,   // pozo en el pueblo / escalera en la cueva
    Count       // NO usar como tile: sirve para dimensionar tablas
};
