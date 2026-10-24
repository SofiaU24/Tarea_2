#include "Metrics.h"
#include "CellularAutomata.h"   // countRegions

// ¿Se puede caminar sobre este tile?
static bool isWalkable(Tile t) {
    return t == Tile::Sand  || t == Tile::Grass || t == Tile::Road ||
           t == Tile::Floor || t == Tile::Ore   || t == Tile::Root ||
           t == Tile::Entrance;
}

static bool isWater(Tile t) {
    return t == Tile::Water || t == Tile::CaveWater;
}

LevelMetrics computeMetrics(const Level& level) {
    LevelMetrics m = level.metrics;      // conserva rooms, trees y genTimeMs que ya se guardaron

    int w = level.tiles.width();
    int h = level.tiles.height();
    int total = w * h;
    if (total == 0) return m;            // nivel vacío: nada que medir

    int water = 0;
    int walkable = 0;
    Grid<uint8_t> floorMask(w, h, 0);    // 1 = celda caminable (para contar regiones)

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Tile t = level.tiles.at(x, y);
            if (isWater(t))    water++;
            if (isWalkable(t)) {
                walkable++;
                floorMask.at(x, y) = 1;
            }
        }
    }

    m.waterPct = 100.0f * water / total;
    m.floorPct = 100.0f * walkable / total;

    // Regiones conectadas: solo tiene sentido en las cuevas.
    if (level.name == "Cuevas")
        m.regions = countRegions(floorMask);

    return m;
}