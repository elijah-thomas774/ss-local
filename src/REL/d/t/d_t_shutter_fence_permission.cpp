#include "d/t/d_t_shutter_fence_permission.h"

#include "d/a/obj/d_a_obj_door_base.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(
    TAG_SHUTTER_FENCE_PERMISSION, dTgShutterFencePermission_c, fProfile::TAG_SHUTTER_FENCE_PERMISSION, 0x2A3, 0, 0
);

dTgShutterFencePermission_c::dTgShutterFencePermission_c() {}

dTgShutterFencePermission_c::~dTgShutterFencePermission_c() {}

int dTgShutterFencePermission_c::create() {}

int dTgShutterFencePermission_c::actorExecute() {
    return SUCCEEDED;
}
