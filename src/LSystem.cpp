#include "LSystem.h"
#include <sstream>

static std::string evolveOnce(const std::string& code, const LRules& rules){
    std::ostringstream next;
    for(char letra : code){
        auto find = rules.find(letra); //busca y guarda la regla correspondiente a la letra

        if(find != rules.end()){ //si find no es final de rules, letra tiene regla
            next << find->second; //second se refiere al reemplazo, segundo par de regla
        } else{
            next << letra;
        }
    }
    return next.str();
}

std::string evolveLSystem(const std::string& axiom, const LRules& rules, int iterations){
    std::string current = axiom;
    for(int i = 0; i < iterations; i++){
        current = evolveOnce(current, rules);
    } 
    return current;
}