#include "LevelTexture.h"
#include <GLFW/glfw3.h>   // incluye las funciones básicas de OpenGL
#include <array>
#include <cstdint>
#include <vector>

namespace {
struct Rgb { uint8_t r, g, b; };

// Color de cada tile. Si agregas un Tile nuevo en core/Tile.h, agrégale su color aquí.
Rgb tileColor(Tile t) {
    switch (t) {
        case Tile::Water:     return {52, 101, 164};
        case Tile::Sand:      return {221, 204, 140};
        case Tile::Grass:     return {96, 160, 72};
        case Tile::Rock:      return {128, 128, 128};
        case Tile::Road:      return {170, 150, 120};
        case Tile::House:     return {178, 72, 60};
        case Tile::Tree:      return {30, 90, 40};
        case Tile::Wall:      return {40, 36, 40};
        case Tile::Floor:     return {150, 130, 110};
        case Tile::Ore:       return {240, 200, 60};
        case Tile::CaveWater: return {40, 70, 130};
        case Tile::Root:      return {110, 70, 40};
        case Tile::Entrance:  return {255, 255, 255};
        default:              return {255, 0, 255};   // magenta = tile sin color asignado
    }
}
}  // namespace

LevelTexture::~LevelTexture() {
    if (m_tex) glDeleteTextures(1, &m_tex);
}

void LevelTexture::update(const Level& level) {
    const Grid<Tile>& tiles = level.tiles;
    if (tiles.empty()) return;
    m_w = tiles.width();
    m_h = tiles.height();

    std::vector<uint8_t> pixels(static_cast<size_t>(m_w) * m_h * 4);   // RGBA
    for (int y = 0; y < m_h; ++y) {
        for (int x = 0; x < m_w; ++x) {
            Rgb c = tileColor(tiles.at(x, y));
            size_t i = (static_cast<size_t>(y) * m_w + x) * 4;
            pixels[i + 0] = c.r;
            pixels[i + 1] = c.g;
            pixels[i + 2] = c.b;
            pixels[i + 3] = 255;
        }
    }

    if (!m_tex) glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // sin suavizar: se ven los píxeles
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}
