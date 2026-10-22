#pragma once
#include "Level.h"

// Convierte un Level en una textura OpenGL (1 píxel por celda) para mostrarla con ImGui::Image.
class LevelTexture {
public:
    LevelTexture() = default;
    ~LevelTexture();
    LevelTexture(const LevelTexture&) = delete;
    LevelTexture& operator=(const LevelTexture&) = delete;

    void update(const Level& level);                  // reconstruye la textura (llamar al regenerar)
    unsigned int id() const { return m_tex; }         // id de la textura OpenGL (0 = aún no existe)
    int width() const  { return m_w; }
    int height() const { return m_h; }

private:
    unsigned int m_tex = 0;
    int m_w = 0;
    int m_h = 0;
};
