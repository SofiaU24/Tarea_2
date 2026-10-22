#include "NoiseGenerator.h"
#include <algorithm>
#include <cmath>
#include <numeric> //std::iota

NoiseGenerator::NoiseGenerator(uint32_t seed) : m_rng(seed) { initPermutation(); }  

void NoiseGenerator::setSeed(uint32_t seed) { m_rng.seed(seed); initPermutation(); }  

void NoiseGenerator::initPermutation(){
    std::array<int, 256> base;
    std::iota(base.begin(), base.end(), 0); // llena la base con numeros de 0 a 255
    std::shuffle(base.begin(), base.end(), m_rng);// mezcla usando el generador aleatorio m_rng

    for (int i = 0; i < 512; ++i) { //la tabla se repite dos veces
        m_p[i] = base[i & 255]; 
    }
}

float NoiseGenerator::fade(float t) {
    float t_5 = std::pow(t, 5); //t^5
    float t_4 = std::pow(t, 4); //t^4
    float t_3 = std::pow(t, 3); //t^3

    return 6*t_5 - 15*t_4 + 10*t_3;
}

float NoiseGenerator::lerp(float t, float a, float b){
    return a + t * (b - a);
}

float NoiseGenerator::grad(int hash, float x, float y){
    switch(hash & 3){
        case 0: return x + y; //(1,1)
        case 1: return -x + y; //(-1,1)
        case 2: return x - y; //(1.-1)
        default: return -x - y; //(-1,-1), defaukt para que cubra el caso 3 o cualquier otro valor
    }
}

float NoiseGenerator::evalPerlin(float x, float y) const {
    int X = static_cast<int>(std::floor(x)) & 255; //entero de la coordenada x
    int Y = static_cast<int>(std::floor(y)) & 255; //entero de la coordenada y

    float xf = x - std::floor(x); //parte fraccionaria de x
    float yf = y - std::floor(y); //parte fraccionaria de y

    float u = fade(xf); //suavizado de la parte fraccionaria de x
    float v = fade(yf); //suavizado de la parte fraccionaria de y

    int aa = m_p[m_p[X] + Y]; //hash para la esquina (X,Y)
    int ab = m_p[m_p[X] + Y + 1]; //hash para la esquina (X,Y+1)
    int ba = m_p[m_p[X + 1] + Y]; //hash para la esquina (X+1,Y)
    int bb = m_p[m_p[X + 1] + Y + 1]; //hash para la esquina (X+1,Y+1)

    float x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1, yf)); //interpolación en x para las esquinas (X,Y) y (X+1,Y)
    float x2 = lerp(u, grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1)); //interpolación en x para las esquinas (X,Y+1) y (X+1,Y+1)

    return (lerp(v, x1, x2) + 1.0f) * 0.5f; //interpolación en y entre los resultados anteriores
}

Grid<float> NoiseGenerator::perlin(int width, int height, float scale){
   Grid<float> grid(width, height);
   for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
        grid.at(x, y) = std::clamp(evalPerlin(x*scale, y*scale), 0.0f, 1.0f); //valor entre 0 y 1
    }
   }
   return grid;
}

Grid<float> NoiseGenerator::fractal(int width, int height, float scale, int octaves, float persistence, float lacunarity){
    Grid<float> grid(width, height);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float sum = 0.0f;
            float norm = 0.0f;
            for(int o = 0; o < octaves; o++){
                sum += evalPerlin(x*scale*frequency, y*scale*frequency) * amplitude;
                norm += amplitude;
                amplitude *= persistence;
                frequency *= lacunarity;
            }
            grid.at(x,y) = sum/norm;
        }
    }
    return grid;
}
/*RESULTADO DE TEST
perlin : min=0.0940171 max=0.849676 media=0.497032
+==--::::--==++++++======+++++++======++**####**+++==--::::--==+
++==------==++****++++=++++****+++==+++**####**++==--:::::--==++
**++==---==+++******+++++++***+++++++++******++=---::....::-==++
##*++=----===++*****+++++++++++++++++++++++++==--:::......:--==+
#**+=---:---==+++****++========================---::.......:--==
*++=--::::--===++*****++==-----========----=======--::....::--==
+==--::::--==+++**###**++=======+++++========+++++==--:::::--==+
==--::::--==++**##%%##**+++===+++****+++==++++***+++==------==++
=---:::--=+***##%%%%##***++++++**####***+++*******++==-----==+++
==------=+**##%%%%%##**++++++**##%%%###*********+++===----===+++
++==---==+**###%###*++=====++**##%%%###****++++++===------======
++==----==++***#***+==---==++*#######**+++++++++===------=======
+==---::--==+++**++===-===++**#####**++=====++++++==---===++++==
==--::::::--==+++++=====+++**####**++===--==+++*++++====+++**+++
=---::...::---=====+++++***#####**+==---:---=++***+++++++*****++
===---:::::::::--==++***#######**+=--:::.:::-==+++++++++*****+++
misma semilla = mismo resultado: SI
fractal: min=0.251353 max=0.731591 media=0.514574
+==----------=======+=+++++++++==++++++++========++=+++++++++===
++======-=======++++++++++++++++++++++++=======+++++++++++++++++
*+++===============++++++++++++++++++++=======++++++++++++++++++
*+++++========++=+++++++++*++++++++============+++++==++++==++++
**+++========++++++++==++++++++=======++++=++===============+++*
*+++=========+++==+++==+++++*+++==+++++++=+=======++++++====++++
+++================+==+++++****+++++++============++++++======++
*+++==================+++******+++++++=======-==============----
++++==================+++***#***+++++++====----==============---
+++====-----===========++********+++*++====---=====+===++++====-
+==---------========+++++*******+++++++++======+==+++++++++++===
==---::::----=-===+++++********++++++++++===++++++++++++**++++==
===---::::----==+++************++++++**++==++++**++++++****++++=
======-------==++********###***+++++++++++++++++***++++*****++++
++=============++*******####****+++===++====++******+++******+++
+====+=======++++++*************+++=====++=++************##**+++
*/