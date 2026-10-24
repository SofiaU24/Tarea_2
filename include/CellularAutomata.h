#pragma once
#include <cstdint>
#include "Grid.h"
#include "Params.h"
#include "Rng.h"

//generar cueva
Grid <uint8_t> generateCellularAutomata(int width, int height, const CaveParams& p, Rng& rng);

//se queda con las regiones más grandes
void keepLargestRegion(Grid<uint8_t>& grid);

int countRegions(const Grid<uint8_t>& grid);