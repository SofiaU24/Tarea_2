#pragma once
#include <cstdlib>
#include "Grid.h"
#include "Tile.h"

// Dibuja una línea de celdas entre (x0,y0) y (x1,y1) con el algoritmo de Bresenham.
// Útil para convertir los segmentos de la tortuga (L-System) en tiles.
// Si onlyOver != Count, solo pisa celdas cuyo tile actual sea 'onlyOver'.
inline void drawLine(Grid<Tile>& grid, int x0, int y0, int x1, int y1, Tile tile,
                     Tile onlyOver = Tile::Count) {
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        if (grid.inBounds(x0, y0) && (onlyOver == Tile::Count || grid.at(x0, y0) == onlyOver))
            grid.at(x0, y0) = tile;
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}
