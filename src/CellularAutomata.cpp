#include "CellularAutomata.h"
#include "Types.h"
#include <queue>
#include <vector>

Grid<uint8_t> generateCellularAutomata(int width, int height, const CaveParams& p, Rng& rng){
    Grid<uint8_t> grid(width, height, 0);

    //parametros
    int iterations = p.iterations;
    int fillProbability = p.fillProbability;
    int neighborsThreshold = p.neighborsThreshold;

    //ruido inicial
    for(int y = 1; y < height - 1; y++){
        for(int x = 1; x < width - 1; x++){
            //decide si celda es suelo (1) o muro (0), segun fillProbability
            grid.at(x, y) = (rng.range(0, 99) < fillProbability) ? 1 : 0;
        }
    }

    for(int i = 0; i < iterations; i++){
        Grid<uint8_t> temp = grid;

        for(int y = 1; y < height - 1; y++){
            for(int x = 1; x < width - 1; x++){
                
                //contar vecinos activos
                int vecinos = 0;
                for(int dy = -1; dy <= 1; dy++){
                    for(int dx = -1; dx <= 1; dx++){
                        if(dx == 0 && dy == 0) continue; //no constarse a si misma
                        if(grid.at(x +dx, y + dy) > 0) vecinos++;
                    }
                }
                temp.at(x, y) = (vecinos >= neighborsThreshold) ? 1 : 0;
            }
        }
        grid = temp;
    }
    return grid;
}
static int floodFill(const Grid<uint8_t>& grid, Grid<int>& label, int sx, int sy, int id) {
    int size = 0;
    std::queue<Point> q;
    q.push({sx, sy});
    label.at(sx, sy) = id;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    while (!q.empty()) {
        Point c = q.front();
        q.pop();
        size++;
        for (int i = 0; i < 4; i++) {
            int nx = c.x + dx[i];
            int ny = c.y + dy[i];
            if (grid.inBounds(nx, ny) && grid.at(nx, ny) == 1 && label.at(nx, ny) == -1) {
                label.at(nx, ny) = id;
                q.push({nx, ny});
            }
        }
    }
    return size;
}

void keepLargestRegion(Grid<uint8_t>& grid) {
    int w = grid.width();
    int h = grid.height();
    Grid<int> label(w, h, -1);
    std::vector<int> sizes;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (grid.at(x, y) == 1 && label.at(x, y) == -1)
                sizes.push_back(floodFill(grid, label, x, y, (int)sizes.size()));

    if (sizes.empty()) return;

    int best = 0;
    for (int i = 1; i < (int)sizes.size(); i++)
        if (sizes[i] > sizes[best]) best = i;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (grid.at(x, y) == 1 && label.at(x, y) != best)
                grid.at(x, y) = 0;
}

int countRegions(const Grid<uint8_t>& grid) {
    int w = grid.width();
    int h = grid.height();
    Grid<int> label(w, h, -1);
    int regions = 0;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (grid.at(x, y) == 1 && label.at(x, y) == -1) {
                floodFill(grid, label, x, y, regions);
                regions++;
            }

    return regions;
}