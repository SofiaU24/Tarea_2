#pragma once
#include <string>
#include <unordered_map>

using LRules = std::unordered_map<char, std::string>; //reglas 

std::string evolveLSystem(const std::string& axiom, const LRules& rules, int iterations); //aplica reglas por cantidad de iteraciones