#ifndef D_T_SHUTTER_FENCE_PERMISSION_H
#define D_T_SHUTTER_FENCE_PERMISSION_H

#include "common.h"
#include "d/t/d_tg.h"

class dTgShutterFencePermission_c : public dTg_c {
public:
    dTgShutterFencePermission_c();
    virtual ~dTgShutterFencePermission_c();

    virtual int create() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    void fn_630_570();
    void fn_630_760();
    void fn_630_8B0();
    void fn_630_8C0();
    void fn_630_8D0();
    void fn_630_8E0();
    void fn_630_8F0();
    void fn_630_900();
    void fn_630_910();
    void fn_630_920();
    void fn_630_930();
    void fn_630_940();
    void fn_630_950();
    void fn_630_960();
    void fn_630_970();
    void fn_630_980();
    void fn_630_990();
    void fn_630_9B0();
    void fn_630_9D0();
    void fn_630_9F0();
    void fn_630_A10();

private:
    /* 0x0FC */ u8 (dTgShutterFencePermission_c::*mGetTimerDuration_fn)();
    /* 0x108 */ void (dTgShutterFencePermission_c::*mFlagBehavior_0x126_fn)();
    /* 0x114 */ void (dTgShutterFencePermission_c::*mFlagBehavior_0x128_fn)();

    /* 0x122 */ u8 field_0x122; ///< Timer Duration
    /* 0x123 */ u8 field_0x123; ///< Timer (-1 means disabled)
    /* 0x124 */ u8 _0x124;
    /* 0x126 */ u16 mFlag_0x126;
    /* 0x128 */ u16 mFlag_0x128;
    /* 0x128 */ u16 _0x12A;
    /* 0x12C */ u8 field_0x12C;
};

#endif
