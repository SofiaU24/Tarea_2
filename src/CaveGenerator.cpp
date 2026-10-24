#include "CaveGenerator.h"         
#include "CellularAutomata.h" //generaar cueva
#include "NoiseGenerator.h" //perlin para minerales
#include "LSystem.h" //reglas raices
#include "Turtle.h" //dibujo raices
#include "Raster.h" 
#include "Rng.h"

#include <cmath>
#include <vector>

Level generateCave(const Params& p, const Level& surface) {                                           
    Level level;                                                // el nivel que devolvemos
    level.name = "Cuevas";                                      // nombre que ve la interfaz
    level.tiles = Grid<Tile>(p.width, p.height, Tile::Wall);    // provisional: todo roca sólida
    level.heightMap = Grid<float>(p.width, p.height, 0.5f);     

    //CA: CUEVAS
    Rng caRng(deriveSeed(p.seed, 4));
    Grid<uint8_t> caves = generateCellularAutomata(p.width, p.height, p.cave, caRng);

    if(p.cave.keepLargestRegion) keepLargestRegion(caves);

    //pasa la grilla de CA
    for(int y = 0; y < p.height; y++){
        for(int x = 0; x < p.width; x++){
            level.tiles.at(x, y) = (caves.at(x, y) == 1)? Tile::Floor : Tile::Wall;
        }
    }

    //PERLIN: MINERALES Y AGUA
    NoiseGenerator noise(deriveSeed(p.seed, 5));
    Grid<float> oreNoise = noise.perlin(p.width, p.height, p.ore.scale);
    for(int y = 0; y < p.height; y++){
        for(int x = 0; x < p.width; x++){
        //minerales y agua se generan solo dentro de cueva
        if(level.tiles.at(x,y) != Tile::Floor) continue;

        float value = oreNoise.at(x,y);
        if(value > p.ore.oreThreshold) level.tiles.at(x, y) = Tile::Ore; //si ruido alto -> mineral
        else if( value < p.ore.waterThreshold) level.tiles.at(x,y) = Tile::CaveWater; // si ruido bajo -> agua de cueva
        }
    }

    //LSYSTEM: RAICES
    LRules rootRules;
    rootRules['F'] = p.roots.ruleF;
    std::string rootText = evolveLSystem(p.roots.axiom, rootRules, p.roots.iterations);

    Rng rootRng(deriveSeed(p.seed, 6));

    for(const Point& base : surface.treeBases){ //una posible rais por cada arbol del pueblo
        if(!rootRng.chance(p.roots.density)) continue; //arbol va a tener raiz o no
        //90° crece hacia abajo
        std::vector<LineSegment> segment = runTurtle(rootText, base.x, base.y, 90.0, p.roots.stepLength, p.roots.angleDeg, p.roots.angleJitterDeg, rootRng);
        for(const LineSegment& s : segment){
            drawLine(level.tiles, (int)(s.x1 + 0.5), (int)(s.y1 + 0.5), (int)(s.x2 + 0.5), (int)(s.y2 + 0.5), Tile::Root, Tile::Floor);
        }
    }
    return level;                                        
}