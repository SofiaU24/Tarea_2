#pragma once                          // evita incluirlo dos veces
#include "Level.h"                    // el tipo Level
#include "Params.h"                   // el tipo Params

// CONTEXTO 2: las cuevas bajo el pueblo. Recibe el pueblo para alinear la entrada y las raíces.
Level generateCave(const Params& params, const Level& surface);