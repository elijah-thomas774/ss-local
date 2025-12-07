#include "d/a/obj/d_a_obj_shutter.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "egg/math/eggMath.h"
#include "m/m3d/m_fanm.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/attention.h"

SPECIAL_ACTOR_PROFILE(SHUTTER, dAcOShutter_c, fProfile::SHUTTER, 0x183, 0, 6);

STATE_DEFINE(dAcOShutter_c, Wait);
STATE_DEFINE(dAcOShutter_c, Open);
STATE_DEFINE(dAcOShutter_c, Close);

struct dShutterAnimResNames {
    char *resName;
    char *mdlName;
    char *anmName;
    char *nodeName;
};
struct dShutterResNames {
    char *resName;
    char *mdlName;
    char *mdlNameTimeshift;
    char *mdlNameFence;
    char *mdlNameFenceTimeshift;
    s32 type;
};

const dShutterAnimResNames sShutterAnimResNames = {
    "ShutterFencedAnim",
    "ShutterFenced",
    "ShutterFenced_Open",
    "ShutterFenced",
};
// clang-format off
const dShutterResNames sShutterResNames[6] = {
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

bool getAngleBetweenPosition(const mVec3_c p0, const mVec3_c &p1, mAng &ang) {
    mVec3_c diff = p1 - p0;

    bool hasAngle = true;
    if (cM::isZero(diff.x) && cM::isZero(diff.z)) {
        hasAngle = false;
    }

    if (hasAngle) {
        ang = mAng::fromVec(diff);
    }

    return hasAngle;
}

bool dAcOShutter_c::createHeap() {
    void *animResPtr = getOarcResFile(sShutterAnimResNames.resName);
    if (animResPtr == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile animRes(animResPtr);
    if (!animRes.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl animMdl = animRes.GetResMdl(sShutterAnimResNames.mdlName);
    if (!animMdl.IsValid()) {
        return false;
    }

    if (!mAnimMdl.create(animMdl, &mAllocator, 0, 1, nullptr)) {
        return false;
    }

    nw4r::g3d::ResAnmChr animAnmChr = animRes.GetResAnmChr(sShutterAnimResNames.anmName);
    if (!animAnmChr.IsValid()) {
        return false;
    }

    if (!mAnmChr.create(animMdl, animAnmChr, &mAllocator, nullptr)) {
        return false;
    }
    mAnmChr.setAnm(mAnimMdl, animAnmChr, m3d::PLAY_MODE_1);
    mAnimMdl.setAnm(mAnmChr);

    s32 type = getFromParams(4, 0x3F);
    if (!isValidType(type)) {
        return false;
    }

    void *mdlResPtr = getOarcResFile(sShutterResNames[type].resName);
    if (mdlResPtr == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile shutterRes(mdlResPtr);
    if (!shutterRes.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl shutterMdl = shutterRes.GetResMdl(sShutterResNames[type].mdlName);
    if (!shutterMdl.IsValid()) {
        return false;
    }
    if (!mShutterMdl.create(shutterMdl, &mAllocator, 0x120, 1, nullptr)) {
        return false;
    }

    if (field_0x601) {
        nw4r::g3d::ResMdl timeshiftMdl = shutterRes.GetResMdl(sShutterResNames[type].mdlNameTimeshift);
        if (!timeshiftMdl.IsValid()) {
            return false;
        }
        TRY_CREATE(mTimeMdl.create(timeshiftMdl, &mAllocator, 0x120, 1, nullptr));
    }

    if (fn_606_3D30()) {
        nw4r::g3d::ResMdl fenceMdl = shutterRes.GetResMdl(sShutterResNames[type].mdlNameFence);
        if (!fenceMdl.IsValid()) {
            return false;
        }

        for (u8 i = 0; i <= ARRAY_LENGTH(mMdlArr0) - 1; ++i) {
            if (!field_0x5F8[i]) {
                continue;
            }
            if (!mMdlArr0[i].create(fenceMdl, &mAllocator, 0x120, 1, nullptr)) {
                return false;
            }
        }

        if (field_0x601) {
            nw4r::g3d::ResMdl timeshiftFenceMdl = shutterRes.GetResMdl(sShutterResNames[type].mdlNameFenceTimeshift);
            if (!timeshiftFenceMdl.IsValid()) {
                return false;
            }
            for (u8 i = 0; i <= ARRAY_LENGTH(mMdlArr1) - 1; ++i) {
                if (!field_0x5F8[i]) {
                    continue;
                }
                if (!mMdlArr1[i].create(timeshiftFenceMdl, &mAllocator, 0x120, 1, nullptr)) {
                    return false;
                }
            }
        }
    }

    return true;
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
