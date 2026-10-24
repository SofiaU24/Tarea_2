#include "TownGenerator.h"
#include "NoiseGenerator.h" //usa perli para generar el terreno
#include "Rng.h" //generador de casas y calles
#include "Bsp.h"
#include "Raster.h" //drawLine
#include "LSystem.h" //reglas arboles
#include "Turtle.h" //dibujo arboles

#include <cmath>
#include <vector>

Level generateTown(const Params& params){
    Level level;
    level.name = "Pueblo";
    level.tiles = Grid<Tile>(params.width, params.height, Tile::Grass); //mapa inicial es pasto
    level.heightMap = Grid<float>(params.width, params.height, 0.5f); 

    NoiseGenerator noise(deriveSeed(params.seed, 1)); //cada etapa us una semilla distinta

    //PERLIN: TERRENO 
    level.heightMap = noise.fractal(params.width, params.height, params.terrain.scale, params.terrain.octaves, params.terrain.persistence, params.terrain.lacunarity);

    //recorre el mapa y asigna altura y tipo de tile segun esta altura
    for(int y = 0; y < params.height; y++){
        for(int x = 0; x < params.width; x++){

            float altura = level.heightMap.at(x,y);

            if(altura < params.terrain.seaLevel){
                level.tiles.at(x,y) = Tile::Water;
            }else if(altura < params.terrain.sandLevel){
                level.tiles.at(x,y) = Tile::Sand;
            }else if (altura > params.terrain.rockLevel){
                level.tiles.at(x,y) = Tile::Rock;
            }else{
                level.tiles.at(x,y) = Tile::Grass;
            }

        }
    }

    Rng rng(deriveSeed(params.seed, 2));
    BspResult bsp = generateBsp(params.width, params.height, params.town.minLotSize, rng);

    //BSP: CASAS Y CALLLES
    int margen = params.town.houseMargin;
    int houseCount = 0;

    for(const Rect& r : bsp.houses){
        //reducido por el margen de cada lado
        int houseX = r.x + margen;
        int houseY = r.y + margen;
        int houseH = r.h - 2*margen;
        int houseW = r.w - 2*margen;

        if(houseW <= 0 || houseH <= 0) continue; //demasiado chico por lo que no genera casa

        for(int y = houseY; y < houseY + houseH; y++){
            for(int x = houseX; x < houseX + houseW; x++){
                if(level.tiles.inBounds(x,y)) level.tiles.at(x,y) = Tile::House; 
            }
        }
        houseCount++;
    }
    level.metrics.rooms = houseCount;

    //Generar calles
    for(const auto& pair : bsp.streets){
        const Rect& a = pair.first;
        const Rect& b = pair.second;
        
        //centro de cada region
        int ax = a.x + a.w /2;
        int ay = a.y + a.h / 2;
        int bx = b.x + b.w / 2;
        int by = b.y + b.h / 2;

        //dibujar pasillo
        drawLine(level.tiles, ax, ay, bx, ay, Tile::Road);
        drawLine(level.tiles, bx, ay, bx, by, Tile::Road);


    }

    //LSYSTEM Y TURTLE: ARBOLES
    LRules treeRules;
    treeRules['F'] = params.trees.ruleF;
    std::string treeText = evolveLSystem(params.trees.axiom, treeRules, params.trees.iterations);

    Rng treeRng(deriveSeed(params.seed, 3));
    int treeCount = 0;
    level.treeBases.clear();

    for(int y = 0; y < params.height; y++){
        for(int x = 0; x < params.width; x++){
            if(level.tiles.at(x, y) != Tile::Grass)continue; //generar arboles solo sobre pasto
            if(!treeRng.chance(params.trees.density)) continue; //probabilidad

            std::vector<LineSegment> segment = runTurtle(treeText, x, y, -90.0, params.trees.stepLength, params.trees.angleDeg, params.trees.angleJitterDeg, treeRng);

            for(const LineSegment& s : segment){
                //drawLine recibe entero, pero turtle trabaja con double. Se usa st::lround para redondear al entero mas cercano 
                drawLine(level.tiles, (int)std::lround(s.x1), (int)std::lround(s.y1), (int)std::lround(s.x2), (int)std::lround(s.y2), Tile::Tree, Tile::Grass);
                level.treeBases.push_back({x, y});
                treeCount++;
            }
        }
    }
    level.metrics.trees = treeCount;


    //devuelve el nivel generado
    return level;
}