## TAREA 2 - Generación Procedural de generación de Cuevas y Terreno. 

Para la generación de el "Pueblo" se uso:
- **Perlin**: Para la generación de paisaje; agua, arena y pasto. 
- **BSP**: Para la generación de casas y sus calles.
- **LSYSTEM**: Para la generación de arboles.

Para la generación de las "Cuevas" se uso:

- **Perlin**: Para los minerales y aguas subterraneas.
- **Automata Celular**: Para la generación de las cuevas en si.
- **LSYSTEM**: Para la generación de las raices de los arboles.

En los archivos "src/" y "include/" se encuentra el codigo fuente. El archivo "imgui/" contiene la librería ImGui.

## Compilación y ejecución
Requiere ´g++´, ´make´, GLFW, GLEW, y OpenGL

´´´bash
sudo apt install build-essential libglfw3-dev libglew-dev
make -j
./Tarea2

## Ventanas
Las pestañas de "Pueblo" y "Cuevas" muestran cada escenario y sus metricas. En el menú de "Controles" a la derecha se puede cambiar la semilla y parámetros de cada técnica.

Link itch.io: [https://itch.io/game/edit/5228937](https://socurv.itch.io/pueblo-y-cuevas-pcg)
