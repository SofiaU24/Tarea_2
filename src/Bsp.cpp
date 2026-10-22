#include "Bsp.h"

BspResult generateBsp(int w, int h, int minSize, Rng& rng) {
    BspResult result;

    std::vector<Rect> stack = { Rect{0, 0, w, h} }; //al inicio todo el mapa es una region

    while(!stack.empty()){
        int size = 0;
        int cut = 0;
        Rect a, b;

        Rect r = stack.back(); //ultima region
        stack.pop_back();

        bool horizontal = rng.chance(0.5f); // true = horizontal, false = vertical

        if(horizontal){
            size = r.h;
            if((size - minSize) <= minSize){ //no cabe otro recorte, por lo que se vuelve hoja
                result.houses.push_back(r); // guarda r como casa
                continue;
            }

            cut = rng.range(minSize, size - minSize);
            a = {r.x, r.y, r.w, cut}; //arriba
            b = {r.x, r.y + cut, r.w, r.h - cut}; //abajo

            result.streets.push_back({a,b});
            stack.push_back(a);
            stack.push_back(b);

        }else{ //vertical
            size = r.w;
            if((size - minSize) <= minSize){ //no cabe otro recorte, por lo que se vuelve hoja
                result.houses.push_back(r); // guarda r como casa
                continue;
            }

            cut = rng.range(minSize, size - minSize);
            a = {r.x, r.y, cut, r.h}; //izquierda
            b = {r.x + cut , r.y, r.w -  cut, r.h}; //derecha-

            result.streets.push_back({a,b});
            stack.push_back(a);
            stack.push_back(b);
        }
    }
    return result;

}