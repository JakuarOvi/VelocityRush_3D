// =============================================================================
//  persistence.cpp
//  Persistent best distance (velocity_rush_best.txt).
// =============================================================================
#include "persistence.h"
#include "game_state.h"

void loadBest() { FILE* f = fopen("velocity_rush_best.txt", "r"); if (f) { if (fscanf(f, "%f", &bestDist) != 1) bestDist = 0; fclose(f); } }
void saveBest() { FILE* f = fopen("velocity_rush_best.txt", "w"); if (f) { fprintf(f, "%.1f\n", bestDist); fclose(f); } }
