#pragma once
#include <string>
#include <vector>
#include "Grid.h"
#include "Tile.h"
#include "Types.h"

// Métricas para la sección de Resultados del informe.
struct LevelMetrics {
    double genTimeMs = 0.0;   // tiempo de generación
    float  waterPct  = 0.0f;  // % de celdas de agua
    float  floorPct  = 0.0f;  // % de celdas caminables
    int    regions   = 0;     // regiones conectadas (cuevas)
    int    rooms     = 0;     // casas / salas
    int    trees     = 0;     // árboles
};

struct Level {
    std::string name;
    Grid<Tile>  tiles;
    Grid<float> heightMap;            // opcional: guarda aquí el Perlin para depurar
    Point       entrance{-1, -1};     // pozo (pueblo) / escalera (cueva); (-1,-1) = sin definir
    std::vector<Point> treeBases;     // base de cada árbol: las raíces de la cueva cuelgan de aquí
    LevelMetrics metrics;
};
