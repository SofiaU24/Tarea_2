#pragma once
#include "LevelTexture.h"
#include "Level.h"
#include "Params.h"

// Estado de la aplicación + interfaz (ImGui).
class App {
public:
    App() = default;

    void drawUI();        // llamar una vez por frame, entre NewFrame() y Render()
    void regenerate();    // genera pueblo y cuevas con los parámetros actuales

private:
    void drawControls();
    void drawLevelView(const Level& level, const LevelTexture& texture);
    void drawMetrics(const Level& level);

    Params m_params;
    Level m_town;
    Level m_cave;
    LevelTexture m_townTex;
    LevelTexture m_caveTex;

    bool m_dirty = true;       // true = hay que regenerar antes de dibujar
    bool m_autoRegen = true;   // regenerar al mover cualquier slider
    int  m_zoom = 6;           // píxeles de pantalla por celda
};
