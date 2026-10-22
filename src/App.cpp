#include "App.h"
#include <cstring>
#include <random>
#include "Timer.h"
#include "imgui.h"
#include "CaveGenerator.h"
#include "Metrics.h"
#include "TownGenerator.h"

namespace {
// Campo de texto para std::string (ImGui trabaja con char[]).
bool inputString(const char* label, std::string& s) {
    char buf[128];
    std::strncpy(buf, s.c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    if (ImGui::InputText(label, buf, sizeof(buf))) {
        s = buf;
        return true;
    }
    return false;
}

// Sliders de un L-System (se reutiliza para árboles y raíces).
bool lsystemControls(LSystemParams& ls) {
    bool c = false;
    c |= inputString("Axioma", ls.axiom);
    c |= inputString("Regla F", ls.ruleF);
    c |= ImGui::SliderInt("Iteraciones", &ls.iterations, 0, 5);   // tope: crece exponencialmente
    c |= ImGui::SliderFloat("Angulo", &ls.angleDeg, 5.0f, 90.0f);
    c |= ImGui::SliderFloat("Largo de paso", &ls.stepLength, 0.5f, 8.0f);
    c |= ImGui::SliderFloat("Variacion de angulo", &ls.angleJitterDeg, 0.0f, 30.0f);
    c |= ImGui::SliderFloat("Densidad", &ls.density, 0.0f, 0.1f, "%.3f");
    return c;
}
}  // namespace

void App::regenerate() {
    {   // Pueblo
        Timer t;
        m_town = generateTown(m_params);
        double ms = t.elapsedMs();
        m_town.metrics = computeMetrics(m_town);
        m_town.metrics.genTimeMs = ms;
    }
    {   // Cuevas (dependen del pueblo: entrada y raíces)
        Timer t;
        m_cave = generateCave(m_params, m_town);
        double ms = t.elapsedMs();
        m_cave.metrics = computeMetrics(m_cave);
        m_cave.metrics.genTimeMs = ms;
    }
    m_townTex.update(m_town);
    m_caveTex.update(m_cave);
}

void App::drawControls() {
    bool changed = false;
    ImGui::Begin("Controles");

    changed |= ImGui::InputInt("Semilla", &m_params.seed);
    if (ImGui::Button("Semilla aleatoria")) {
        m_params.seed = static_cast<int>(std::random_device{}() & 0x7fffffff);
        changed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Regenerar")) m_dirty = true;
    ImGui::Checkbox("Regenerar al mover sliders", &m_autoRegen);
    changed |= ImGui::SliderInt("Ancho", &m_params.width, 32, 200);
    changed |= ImGui::SliderInt("Alto", &m_params.height, 32, 200);
    ImGui::SliderInt("Zoom", &m_zoom, 1, 12);   // solo visual: no regenera

    if (ImGui::CollapsingHeader("Paisaje - Perlin (terreno)", ImGuiTreeNodeFlags_DefaultOpen)) {
        TerrainParams& t = m_params.terrain;
        changed |= ImGui::SliderFloat("Escala", &t.scale, 0.005f, 0.2f, "%.3f");
        changed |= ImGui::SliderInt("Octavas", &t.octaves, 1, 8);
        changed |= ImGui::SliderFloat("Persistence", &t.persistence, 0.1f, 0.9f);
        changed |= ImGui::SliderFloat("Lacunarity", &t.lacunarity, 1.5f, 3.0f);
        changed |= ImGui::SliderFloat("Nivel del mar", &t.seaLevel, 0.0f, 1.0f);
        changed |= ImGui::SliderFloat("Nivel de arena", &t.sandLevel, 0.0f, 1.0f);
        changed |= ImGui::SliderFloat("Nivel de roca", &t.rockLevel, 0.0f, 1.0f);
    }
    if (ImGui::CollapsingHeader("Mazmorras - BSP (pueblo)")) {
        changed |= ImGui::SliderInt("Tamano minimo de lote", &m_params.town.minLotSize, 5, 40);
        changed |= ImGui::SliderInt("Margen de casa", &m_params.town.houseMargin, 0, 4);
    }
    if (ImGui::CollapsingHeader("Secuencias - L-System (arboles)")) {
        ImGui::PushID("trees");
        changed |= lsystemControls(m_params.trees);
        ImGui::PopID();
    }
    if (ImGui::CollapsingHeader("Mazmorras - Automata celular (cuevas)")) {
        CaveParams& c = m_params.cave;
        changed |= ImGui::SliderInt("% de suelo inicial", &c.fillProbability, 20, 80);
        changed |= ImGui::SliderInt("Iteraciones", &c.iterations, 0, 10);
        changed |= ImGui::SliderInt("Umbral de vecinos", &c.neighborsThreshold, 1, 8);
        changed |= ImGui::Checkbox("Quedarse con la region mas grande", &c.keepLargestRegion);
    }
    if (ImGui::CollapsingHeader("Paisaje - Perlin (minerales y agua)")) {
        OreParams& o = m_params.ore;
        changed |= ImGui::SliderFloat("Escala##ore", &o.scale, 0.02f, 0.4f);
        changed |= ImGui::SliderFloat("Umbral mineral", &o.oreThreshold, 0.0f, 1.0f);
        changed |= ImGui::SliderFloat("Umbral agua", &o.waterThreshold, 0.0f, 1.0f);
    }
    if (ImGui::CollapsingHeader("Secuencias - L-System (raices)")) {
        ImGui::PushID("roots");
        changed |= lsystemControls(m_params.roots);
        ImGui::PopID();
    }
    ImGui::End();

    if (changed && m_autoRegen) m_dirty = true;
}

void App::drawMetrics(const Level& level) {
    const LevelMetrics& m = level.metrics;
    ImGui::Text("Tiempo de generacion: %.2f ms", m.genTimeMs);
    ImGui::Text("Agua: %.1f %%   Caminable: %.1f %%", m.waterPct, m.floorPct);
    ImGui::Text("Regiones: %d   Salas/casas: %d   Arboles: %d", m.regions, m.rooms, m.trees);
}

void App::drawLevelView(const Level& level, const LevelTexture& texture) {
    drawMetrics(level);
    if (texture.id() == 0) return;
    ImVec2 size(static_cast<float>(texture.width() * m_zoom),
                static_cast<float>(texture.height() * m_zoom));
    ImGui::BeginChild("scroll", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Image((ImTextureID)(intptr_t)texture.id(), size);
    ImGui::EndChild();
}

void App::drawUI() {
    drawControls();
    if (m_dirty) {          // regenerar ANTES de dibujar las vistas
        regenerate();
        m_dirty = false;
    }

    ImGui::Begin("Vista");
    if (ImGui::BeginTabBar("niveles")) {
        if (ImGui::BeginTabItem("Pueblo (superficie)")) {
            drawLevelView(m_town, m_townTex);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Cuevas (subsuelo)")) {
            drawLevelView(m_cave, m_caveTex);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
