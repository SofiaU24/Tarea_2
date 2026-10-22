#pragma once                          // evita incluirlo dos veces
#include "Level.h"                    // el tipo Level y LevelMetrics

// Calcula las métricas de un nivel ya generado (el tiempo lo mide App, no esta función).
LevelMetrics computeMetrics(const Level& level);