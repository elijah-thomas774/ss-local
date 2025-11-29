#include "d/t/d_t_shutter_fence_forbiddance.h"

#include "common.h"
#include "d/a/obj/d_a_obj_door_base.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(
    TAG_SHUTTER_FENCE_FORBIDDANCE, dTgShutterFenceForbiddance_c, fProfile::TAG_SHUTTER_FENCE_FORBIDDANCE, 0x2A4, 0, 4
);

dTgShutterFenceForbiddance_c *dTgShutterFenceForbiddance_c::spInstance;

dTgShutterFenceForbiddance_c::dTgShutterFenceForbiddance_c() {
    if (!spInstance) {
        spInstance = this;
    }
}
dTgShutterFenceForbiddance_c::~dTgShutterFenceForbiddance_c() {
    if (this == spInstance) {
        spInstance = nullptr;
    }
}

int dTgShutterFenceForbiddance_c::create() {
    if (this != spInstance) {
        return FAILED;
    }
    return SUCCEEDED;
}

int dTgShutterFenceForbiddance_c::actorExecute() {
    for (dAcObjDoor_c *pDoor = dAcObjDoor_c::getNextDoorInList(nullptr); pDoor != nullptr;
         pDoor = dAcObjDoor_c::getNextDoorInList(pDoor)) {
        pDoor->vt_0x8C();
    }
    return SUCCEEDED;
}
