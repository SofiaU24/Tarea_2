#pragma once
#include <string>
#include <vector>
#include "Rng.h"

//segmento dibujado
struct LineSegment{
    double x1, y1;
    double x2, y2;
};
//estado de tortuga
struct TurtleState{
    double x, y;
    double angle;
};

//interpreta texto de LSystem y devuelve el dibujo
std::vector<LineSegment> runTurtle(const std::string& instrucciones, double x, double y, double anguloInicial, double paso, double anguloDeg, double jitterDeg, Rng& rng);