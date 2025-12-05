#include "d/a/obj/d_a_obj_shutter.h"

#include "common.h"
#include "toBeSorted/attention.h"

SPECIAL_ACTOR_PROFILE(SHUTTER, dAcOShutter_c, fProfile::SHUTTER, 0x183, 0, 6);

STATE_DEFINE(dAcOShutter_c, Wait);
STATE_DEFINE(dAcOShutter_c, Open);
STATE_DEFINE(dAcOShutter_c, Close);

struct dShutterAnimResNames {
    const char *resName;
    const char *mdlName;
    const char *anmName;
};
struct dShutterResNames {
    const char *resName;
    const char *mdlName;
    const char *mdlNameTimeshift;
    const char *mdlNameFence;
    const char *mdlNameFenceTimeshift;
    s32 type;
};
const dShutterAnimResNames sShutterAnimResNames[] = {
    "ShutterFencedAnim",
    "ShutterFenced",
    "ShutterFenced_Open",
};
// clang-format off
const dShutterResNames sShutterResNames[] = {
    {"ShutterFenced00",  "ShutterFenced00",            nullptr,  "ShutterFencedFence00",                 nullptr, 0}, // AC/SV
    {"ShutterFenced01",  "ShutterFenced01",            nullptr,  "ShutterFencedFence01",                 nullptr, 1}, // ET
    {"ShutterFenced02", "ShutterFenced02T", "ShutterFenced02N", "ShutterFencedFence02T", "ShutterFencedFence02N", 2}, // LMF
    {"ShutterFenced03",  "ShutterFenced03",            nullptr,  "ShutterFencedFence03",                 nullptr, 3}, // FS
    {"ShutterFenced04",  "ShutterFenced04",            nullptr,  "ShutterFencedFence04",                 nullptr, 4}, // SkyKeep
    {"ShutterFenced05",  "ShutterFenced05",            nullptr,                 nullptr,                 nullptr, 5}, // Unused?
};
// clang-format on

const InteractionTargetDef sShutterOpen = {
    0, 0x2, 0x1, OPEN, 0, 10000.f, 90.f, 0.f, -100.f, 100.f, 50.f, 1.f,
};

const char *const ShutterUnlocked = "ShutterUnlocked";
const char *const Shutter = "Shutter";

const char *getEventName() {
    return Shutter;
}
bool isValidType(s32 type) {
    return 0 <= type && type <= (int)ARRAY_LENGTH(sShutterResNames) - 1;
}

u32 createEyeParam(u8 flag1, u8 flag2) {
    return (flag1 << 4) | (flag2 << 12);
}

bool dAcOShutter_c::canOpen() {
    // Not implemented - Used to emit dtor
    return true;
}

void dAcOShutter_c::initializeState_Wait() {}
void dAcOShutter_c::executeState_Wait() {}
void dAcOShutter_c::finalizeState_Wait() {}
void dAcOShutter_c::initializeState_Open() {}
void dAcOShutter_c::executeState_Open() {}
void dAcOShutter_c::finalizeState_Open() {}
void dAcOShutter_c::initializeState_Close() {}
void dAcOShutter_c::executeState_Close() {}
void dAcOShutter_c::finalizeState_Close() {}
