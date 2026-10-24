#include "Turtle.h"
#include <cmath>
#include <stack>

#ifndef M_PI
#define M_PI 3.1416
#endif

std::vector<LineSegment> runTurtle(const std::string& instrucciones, double x, double y, double anguloInicial, double paso, double anguloDeg, double jitterDeg, Rng& rng){
    std::vector<LineSegment> path;
    std::stack<TurtleState> history;

    TurtleState state{x, y, anguloInicial * (M_PI / 180.0)}; //de grados a radianes
    double deltaRad = anguloDeg * (M_PI / 180.0);

    for(char i : instrucciones){
        double jitterRad = (rng. next01() * 2.0 - 1.0) * jitterDeg * (M_PI / 180.0); //variacion al azar del giro

        switch(i){
            case 'F': {//avanzar dibujando
                double nextx = state.x + paso * std::cos(state.angle);
                double nexty = state.y + paso * std::sin(state.angle);
                path.push_back({state.x, state.y, nextx, nexty});
                state.x = nextx;
                state.y = nexty;
                break;
            }
            case 'f':{ //avanzar sin dibujar
                state.x += paso * std::cos(state.angle);
                state.y += paso * std::sin(state.angle);
                break;
            }
            case '+': //giro derecha
                state.angle += deltaRad + jitterRad; 
                break;
            case '-': //giro izquierda
                state.angle -= deltaRad + jitterRad;
                break;
            case '[': //guardar estado
                history.push(state);
                break; //restaurar estado
            case ']':
                if(!history.empty()){
                    state = history.top();
                    history.pop();
                }
                break;
            default: 
                break;
        }
    }
    return path;
}