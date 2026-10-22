#pragma once
#include <string>

// Todos los valores que el menú (ImGui) puede modificar.

struct TerrainParams {            // Perlin fractal (paisaje)
    float scale       = 0.05f;
    int   octaves     = 4;
    float persistence = 0.5f;
    float lacunarity  = 2.0f;
    float seaLevel    = 0.35f;    // altura < seaLevel  -> agua
    float sandLevel   = 0.40f;    // altura < sandLevel -> arena
    float rockLevel   = 0.80f;    // altura > rockLevel -> roca
};

struct TownParams {               // BSP (estructuras del pueblo)
    int minLotSize = 10;          // tamaño mínimo de un lote
    int houseMargin = 1;          // espacio entre el borde del lote y la casa
};

struct LSystemParams {            // L-System (reutilizado para árboles y raíces)
    std::string axiom = "F";
    std::string ruleF = "F[+F]F[-F]F";
    int   iterations  = 3;        // ¡cuidado!, crece exponencialmente
    float angleDeg    = 25.0f;
    float stepLength  = 2.0f;
    float angleJitterDeg = 0.0f;
    float density     = 0.01f;    // árboles: fracción de celdas de pasto con árbol / raíces: idem
};

struct CaveParams {               // Autómata celular (cuevas)
    int  fillProbability    = 55;
    int  iterations         = 4;
    int  neighborsThreshold = 5;
    bool keepLargestRegion  = true;
};

struct OreParams {                // Perlin dentro de la cueva
    float scale          = 0.12f;
    float oreThreshold   = 0.70f; // ruido > umbral  -> mineral
    float waterThreshold = 0.25f; // ruido < umbral  -> agua subterránea
};

struct Params {
    int seed   = 1234;
    int width  = 96;
    int height = 96;

    TerrainParams terrain;
    TownParams    town;
    LSystemParams trees;
    LSystemParams roots;
    CaveParams    cave;
    OreParams     ore;

    Params() {
        roots.ruleF = "F[+F][-F]F";   // otra gramática por defecto para las raíces
        roots.iterations = 2;
        roots.density = 0.5f;
    }
};
