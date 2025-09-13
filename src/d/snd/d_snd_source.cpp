#include "d/snd/d_snd_source.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_source_mgr.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"
#include "sized_string.h"

struct d_snd_mgr_unk_6_sinit {
    d_snd_mgr_unk_6_sinit() : field_0x00(0), field_0x04(0.0f) {}

    u32 field_0x00;
    f32 field_0x04;
};

d_snd_mgr_unk_6_sinit d_snd_mgr_unk_6_sinit_instance;

const char *help_i_need_data() {
    return "%s_%s_%d";
}

dSoundSource_c::dSoundSource_c(u8 sourceType, dAcBase_c *actor, const char *name, dSndSourceGroup_c *pOwnerGroup)
    : dSnd3DActor_c(pOwnerGroup->getAmbientParam(), sourceType),
      mpName(name),
      field_0x0F4(0),
      mpActor(actor),
      mSourceType(sourceType),
      field_0x0FE(0),
      field_0x0FF(0),
      field_0x100(0),
      field_0x101(0),
      field_0x102(0),
      field_0x104(0),
      field_0x108(0),
      field_0x10C(0),
      field_0x11C(0),
      mpOwnerGroup(pOwnerGroup),
      field_0x154(0),
      field_0x158(-1),
      field_0x15A(-1) {
    mSourceCategory = dSndSourceMgr_c::getSourceCategoryForSourceType(sourceType, name);
    // TODO: Offsetof
    nw4r::ut::List_Init(&field_0x110, 0xEC);
    nw4r::ut::List_Init(&field_0x120, 0x04);
    nw4r::ut::List_Init(&field_0x12C, 0x04);
    pOwnerGroup->registerSource(this);
}

dSoundSource_c::~dSoundSource_c() {
    SetUserParam(0);
    vt_0x44();
    d_s_vt_0x1BC();
    dSndSourceMgr_c::GetInstance()->unregisterSource(this);
    mpOwnerGroup->unregisterSource(this);
}

const nw4r::math::VEC3 &dSoundSource_c::getListenerPosition() const {
    return dSnd3DManager_c::GetInstance()->getSndListenerPos();
}

bool dSoundSource_c::hasPlayingSounds() const {
    return dSnd3DActor_c::hasPlayingSounds();
}

bool dSoundSource_c::isPlayingSound(u32 soundId) {
    return dSnd3DActor_c::isPlayingSound(soundId);
}

bool dSoundSource_c::isPlayingSound(const char *soundId) {
    return isPlayingSound(soundLabelToSoundId(soundId));
}

void dSoundSource_c::d_vt_0x38(bool flag, int fadeFrames) {
    if (flag == 0) {
        PauseAllSound(flag, fadeFrames);
        d_s_vt_0x1C4(flag, fadeFrames);
        a_field_0x80 = 0;
    } else if (a_field_0x7E == 0) {
        PauseAllSound(flag, fadeFrames);
        d_s_vt_0x1C4(flag, fadeFrames);
        a_field_0x80 = 1;
    }
}

s32 dSoundSource_c::getRoomId() const {
    return mpActor->roomid;
}

nw4r::snd::SoundStartable::StartResult
dSoundSource_c::SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *) {
    // TODO
    return START_SUCCESS;
}

void dSoundSource_c::d_vt_0x58() {
    // noop
}

void dSoundSource_c::d_vt_0x5C() {
    // noop
}

u32 dSoundSource_c::getCharacterTalkSoundId(u32 baseSoundId, dSoundSource_c *source) {
    if (baseSoundId != -1 && source != nullptr) {
        SizedString<64> label;

        // maybe an inline
        const char *baseLabel = nullptr;
        if (dSndMgr_c::GetInstance()->getArchive() != nullptr) {
            baseLabel = dSndMgr_c::GetInstance()->getArchive()->GetSoundLabelString(baseSoundId);
        }

        const char *charLabel = source->getName();
        label.sprintf("%s_%s", baseLabel, charLabel);
        u32 newLabel = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (newLabel != -1) {
            return newLabel;
        }
    }
    return baseSoundId;
}

u32 dSoundSource_c::getRemoConSoundVariant(u32 soundId) const {
    const char *label = soundIdToSoundLabel(soundId);
    SizedString<64> str;
    str.sprintf("%s_RC", label);
    return soundLabelToSoundId(str);
}
