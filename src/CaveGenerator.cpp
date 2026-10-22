#include "CaveGenerator.h"            // su propio header

Level generateCave(const Params& p, const Level& surface) {
    (void)surface;                                              // aún no usamos el pueblo (evita el warning)
    Level level;                                                // el nivel que devolvemos
    level.name = "Cuevas";                                      // nombre que ve la interfaz
    level.tiles = Grid<Tile>(p.width, p.height, Tile::Wall);    // provisional: todo roca sólida
    level.heightMap = Grid<float>(p.width, p.height, 0.5f);     // alturas (sin uso por ahora)
    return level;                                               // se completará en el paso 8
}