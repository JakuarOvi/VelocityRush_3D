// =============================================================================
//  persistence.cpp
//  Best-distance save and validated optional physics configuration.
// =============================================================================
#include "persistence.h"
#include "game_state.h"

void loadBest() { FILE* f = fopen("velocity_rush_best.txt", "r"); if (f) { if (fscanf(f, "%f", &bestDist) != 1 || bestDist < 0.0f) bestDist = 0.0f; fclose(f); } }
void saveBest() { FILE* f = fopen("velocity_rush_best.txt", "w"); if (f) { fprintf(f, "%.1f\n", bestDist); fclose(f); } }

static float bounded(float v, float lo, float hi) { return isfinite(v) ? clampf(v, lo, hi) : lo; }
void loadPhysicsConfig() {
    FILE* f = fopen("velocity_rush_physics.cfg", "r");
    if (!f) return;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        char key[64]; float v;
        if (line[0] == '#' || sscanf(line, " %63[^=]=%f", key, &v) != 2) continue;
        if (!strcmp(key, "brake_decel")) physics.brakeDecel = bounded(v, 5.0f, 45.0f);
        else if (!strcmp(key, "rolling_resistance")) physics.rollingResistance = bounded(v, 0.0f, 3.0f);
        else if (!strcmp(key, "aero_drag_scale")) physics.aeroDragScale = bounded(v, 0.25f, 2.5f);
        else if (!strcmp(key, "dry_grip_scale")) physics.dryGripScale = bounded(v, 0.4f, 1.5f);
        else if (!strcmp(key, "wet_grip_scale")) physics.wetGripScale = bounded(v, 0.25f, 1.0f);
        else if (!strcmp(key, "max_lateral_accel")) physics.maxLateralAccel = bounded(v, 3.0f, 14.0f);
        else if (!strcmp(key, "steering_response")) physics.steeringResponse = bounded(v, 2.0f, 18.0f);
        else if (!strcmp(key, "lateral_response")) physics.lateralResponse = bounded(v, 2.0f, 16.0f);
        else if (!strcmp(key, "drift_threshold")) physics.driftThreshold = bounded(v, 0.08f, 0.8f);
        else if (!strcmp(key, "drift_grip_scale")) physics.driftGripScale = bounded(v, 0.35f, 1.0f);
        else if (!strcmp(key, "low_speed_steer_floor")) physics.lowSpeedSteerFloor = bounded(v, 0.05f, 0.6f);
        else if (!strcmp(key, "suspension_stiffness")) physics.suspensionStiffness = bounded(v, 1.0f, 12.0f);
        else if (!strcmp(key, "offroad_decel")) physics.offroadDecel = bounded(v, 4.0f, 35.0f);
        else if (!strcmp(key, "collision_decel")) physics.collisionDecel = bounded(v, 4.0f, 30.0f);
    }
    fclose(f);
}
