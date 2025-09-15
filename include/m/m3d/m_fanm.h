#ifndef M3D_M_FANM_H
#define M3D_M_FANM_H

#include "m/m3d/m_banm.h"

namespace m3d {

enum playMode_e {
    PLAY_MODE_0,
    PLAY_MODE_1,
    PLAY_MODE_2,
    PLAY_MODE_3,
    PLAY_MODE_4,
};

class fanm_c : public banm_c {
public:
    fanm_c();
    virtual ~fanm_c();

    virtual int getType() const = 0;
    virtual void play();

    void set(f32, playMode_e, f32, f32);
    void set2(f32, playMode_e, f32, f32, f32);
    void setFrame(f32);
    void setFrameOnly(f32);
    bool isStop() const;
    bool checkFrame(f32) const;
    bool unk_802EAE70() const;

    inline void setPlayState(playMode_e state) {
        mPlayState = state;
    }

    playMode_e getPlayMode() const {
        return (playMode_e)mPlayState;
    }

    inline f32 getEndFrame() {
        return mEndFrame;
    }

    // Something about this is wrong
    inline void setStartFrame(f32 f) {
        mEndFrame = f;
    }

    inline f32 getStartFrame() {
        return mStartFrame;
    }

private:
    /* 0x28 */ f32 mEndFrame;
    /* 0x2C */ f32 mStartFrame;
    /* 0x30 */ f32 mCurrentFrame;
    /* 0x34 */ u8 mPlayState;
};

} // namespace m3d

#endif
