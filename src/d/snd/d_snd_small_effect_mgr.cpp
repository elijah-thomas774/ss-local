#include "d/snd/d_snd_small_effect_mgr.h"

#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "rvl/OS/OSFastCast.h"
#include "toBeSorted/music_mgrs.h"

SND_DISPOSER_DEFINE(dSndSmallEffectMgr_c)

dSndSmallEffectMgr_c::dSndSmallEffectMgr_c()
    : field_0x10(0), mTextboxAdvanceSound(-1), field_0x40(0), field_0x42(0), field_0x44(0) {
    for (int i = 0; i < NUM_DELAYED_SOUNDS; i++) {
        mDelayedSoundIds[i] = -1;
        mDelayedSoundTimers[i] = 0;
    }
}

void dSndSmallEffectMgr_c::calc() {
    if (!dSndPlayerMgr_c::GetInstance()->checkFlag(0x4)) {
        for (int i = 0; i < NUM_DELAYED_SOUNDS; i++) {
            if (mDelayedSoundIds[i] != -1) {
                mDelayedSoundTimers[i]--;
                if (mDelayedSoundTimers[i] <= 0) {
                    playSound(mDelayedSoundIds[i], nullptr);
                    mDelayedSoundIds[i] = -1;
                }
            }
        }
    }
}

bool dSndSmallEffectMgr_c::playSound(u32 soundId) {
    if (soundId >= SE_S_BUTTON_CALL_L && soundId <= SE_S_PLAY_GUIDE_BUTTON_BLINK) {
        soundId = SE_S_BUTTON_CALL_L;
    } else if (soundId >= SE_S_BUTTON_CALL_R && soundId <= SE_S_2_BUTTON_BLINK) {
        soundId = SE_S_BUTTON_CALL_R;
    } else {
        switch (soundId) {
            case SE_S_BOSS_KEY_TRANS: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BOSS_KEY_TRANS_OFF, 5);
                break;
            }
            case SE_S_BOSS_KEY_TRANS_OFF: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BOSS_KEY_TRANS, 5);
                break;
            }
            case SE_S_HP_GAUGE_UP: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HEART_PIECE_GET)) {
                    return false;
                }
                break;
            }
            case SE_S_HEART_PIECE_GET: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HP_GAUGE_UP)) {
                    return false;
                }
                break;
            }
            case SE_S_MAP_OPEN:
            case SE_S_MENU_IN:  {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_HELP_OUT, 1);
                break;
            }
            case SE_S_HELP_OUT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_IN) ||
                    isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MAP_OPEN)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_SELECT_TURN_PAGE_LEFT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_LEFT)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_SELECT_TURN_PAGE_RIGHT: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_RIGHT)) {
                    return false;
                }
                break;
            }
            case SE_S_ITEM_SELECT_START: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_BM_KEEP_ERROR)) {
                    soundId = SE_S_ITEM_SELECT_START_WAIT;
                }
                break;
            }
            case SE_S_SKIP: {
                if (fn_80364DA0(ENEMY_SOUND_MGR)) {
                    fn_80365020(ENEMY_SOUND_MGR);
                }
                break;
            }
            case SE_S_WINDOW_PRESS_A: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT, SE_S_POINTER_OK)) {
                    return false;
                }
                break;
            }
            case SE_S_MENU_P1_HOLD_POINTER: {
                stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_P1_POINT_ITEM, 0);
                break;
            }
            case SE_S_MENU_P1_POINT_ITEM: {
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_MENU_P1_HOLD_POINTER)) {
                    return false;
                }
                break;
            }
            case SE_S_FIRST_PERSON_ON:
            case SE_S_FIRST_PERSON_OFF: {
                if (!isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_DOWSING_SELECT_START)) {
                    playSound(SE_S_DOWSING_SELECT_START);
                }
                break;
            }
            case SE_S_DOWSING_WAIT:
                if (isPlayingSound(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_FIRST_PERSON_ON)) {
                    return false;
                }
                break;
        }
    }

    return playSoundInternal(soundId);
}

bool dSndSmallEffectMgr_c::playSoundWithPan(u32 soundId, f32 pan) {
    bool ok = playSoundInternal(soundId);
    if (ok) {
        if (pan > 1.0f) {
            pan = 1.0f;
        } else if (pan < -1.0f) {
            pan = -1.0f;
        }
        mNormalSound.SetPan(pan);
    }
    return ok;
}

nw4r::snd::SoundHandle *dSndSmallEffectMgr_c::getHoldSoundHandle(u32 soundId) {
    // Find an existing handle holding this sound
    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        if (h->GetId() == soundId) {
            return h;
        }
    }

    // Find a free handle
    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        if (!h->IsAttachedSound()) {
            return h;
        }
    }

    // Drop a lower-priority sound
    nw4r::snd::SoundHandle *least = nullptr;
    nw4r::snd::SoundArchive::SoundInfo info;
    dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(soundId, &info);
    s32 newPriority = info.playerPriority;

    for (int i = 0; i < NUM_HOLD_SOUNDS; i++) {
        nw4r::snd::SoundHandle *h = &mHoldSoundHandles[i];
        dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(h->GetId(), &info);
        if (info.playerPriority < newPriority) {
            newPriority = info.playerPriority;
            least = h;
        }
    }
    return least;
}

bool dSndSmallEffectMgr_c::playSkbSound(u32 soundId) {
    switch (soundId) {
        case SE_S_SK_POINT:
        case SE_S_SK_INPUT:
            if (isPlayingSound(SE_S_SK_INPUT_DECIDE)) {
                return false;
            }
            break;
        case SE_S_SK_INPUT_DECIDE:
            stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_POINT, 0);
            stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_INPUT, 0);
            break;
        case SE_S_SK_DELETE_ERROR: stopSounds(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL, SE_S_SK_INPUT, 0); break;
    }
    return playSound(soundId);
}

void dSndSmallEffectMgr_c::stopSounds(u32 playerIdx, u32 soundId, s32 fadeFrames) {
    SoundStopper stopper(soundId, fadeFrames);
    dSndControlPlayerMgr_c::GetInstance()->getPlayer1(playerIdx)->ForEachSound(stopper, false);
}

void dSndSmallEffectMgr_c::stopSounds(u32 soundId, s32 fadeFrames) {
    for (s32 playerId = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; playerId <= dSndControlPlayerMgr_c::sPlayerMax;
         playerId++) {
        stopSounds(playerId, soundId, fadeFrames);
    }
}

bool dSndSmallEffectMgr_c::isPlayingSound(u32 playerIdx, u32 soundId) {
    bool isPlaying = false;
    IsCurrentSoundIdChecker check(soundId, &isPlaying);
    dSndControlPlayerMgr_c::GetInstance()->getPlayer1(playerIdx)->ForEachSound(check, false);
    return isPlaying;
}

bool dSndSmallEffectMgr_c::isPlayingSound(u32 soundId) {
    for (s32 playerId = dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT; playerId <= dSndControlPlayerMgr_c::sPlayerMax;
         playerId++) {
        if (isPlayingSound(playerId, soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndSmallEffectMgr_c::playButtonPressSoundWhenAdvancingTextBoxes(f32 ratio) {
    if (mTextboxAdvanceSound == -1) {
        return false;
    }

    bool ok = playSound(mTextboxAdvanceSound);
    if (ok) {
        s16 varValue;
        f32 value = ratio * 100.0f;
        OSf32tos16(&value, &varValue);
        nw4r::snd::SeqSoundHandle handle(&mNormalSound);
        handle.WriteVariable(10, varValue);
    }

    return ok;
}

void dSndSmallEffectMgr_c::resetButtonPressSound() {
    mTextboxAdvanceSound = -1;
}

void dSndSmallEffectMgr_c::setButtonPressSound(dSoundSource_c *source) {
    resetButtonPressSound();
    if (source != nullptr) {
        mTextboxAdvanceSound = dSoundSource_c::modifySoundId(SE_S_TALK_CHAR, source);
        if (mTextboxAdvanceSound == SE_S_TALK_CHAR) {
            resetButtonPressSound();
        }
    }
}

bool dSndSmallEffectMgr_c::playBattleHitSound(BattleHitSound_e type, dSoundSource_c *source) {
    // if we're not in battle, don't play any of the hit effects
    if (!dSndBgmMgr_c::GetInstance()->isPlayingAnyBattleMusic()) {
        return false;
    }

    if (source != nullptr) {
        const char *name = source->getName();
        s32 sourceType = source->getSourceType();
        if (sourceType == SND_SOURCE_OBJECT_40) {
            return false;
        }

        switch (type) {
            case BATTLE_TUTTI_GUARDJUST:
                if (sourceType >= SND_SOURCE_BULLET) {
                    // Do not play battle effects for countering bullets
                    return false;
                }
                break;
            case BATTLE_TUTTI_FINISH:
                if (streq(name, "BLasBos")) {
                    // Do not play finish effect for finishing Demise
                    return false;
                }
                break;
            default: break;
        }
    }
    u32 soundId = BGM_BATTLE_TUTTI;
    switch (type) {
        case BATTLE_TUTTI_TURN:      soundId = BGM_BATTLE_TUTTI_TURN; break;
        case BATTLE_TUTTI_JUMP:      soundId = BGM_BATTLE_TUTTI_JUMP; break;
        case BATTLE_TUTTI_FINISH:    soundId = BGM_BATTLE_TUTTI_FINISH; break;
        case BATTLE_TUTTI_GUARDJUST: soundId = BGM_BATTLE_TUTTI_GUARDJUST; break;
        default:                     break;
    }

    if (mBattleTuttiHandle.IsAttachedSound()) {
        u32 alreadyPlayingSound = mBattleTuttiHandle.GetId();
        // BGM_BATTLE_TUTTI_ sounds are ordered by priority apparently
        if (alreadyPlayingSound > soundId) {
            return false;
        }
        if (alreadyPlayingSound == BGM_BATTLE_TUTTI) {
            nw4r::snd::SeqSoundHandle handle(&mBattleTuttiHandle);
            // Do not allow stopping BGM_BATTLE_TUTTI too early
            if ((s32)handle.GetTick() < 12) {
                return false;
            }
        }
        mBattleTuttiHandle.Stop(5);
    }

    // TODO ...

    return true;
}
