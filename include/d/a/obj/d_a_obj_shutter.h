#ifndef D_A_OBJ_SHUTTER_H
#define D_A_OBJ_SHUTTER_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_door_base.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_smdl.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_flow_mgr.h"

class dAcOShutter_c : public dAcObjDoor_c {
public:
    dAcOShutter_c() : mStateMgr(*this, sStateID::null), mEventRelated(*this, 0) {}
    virtual ~dAcOShutter_c() {}

    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void doInteraction(s32 /* Interaction */) override;

    virtual bool canOpen() override;        // in Main Dol
    virtual bool hasVisitedRoom() override; // in Main Dol
    virtual void vt_0x88() override;
    virtual void vt_0x8C() override;

    STATE_FUNC_DECLARE(dAcOShutter_c, Wait);
    STATE_FUNC_DECLARE(dAcOShutter_c, Open);
    STATE_FUNC_DECLARE(dAcOShutter_c, Close);

    bool fn_606_3D30();

private:
    /* 0x33C */ m3d::smdl_c mAnimMdl;
    /* 0x358 */ m3d::smdl_c mShutterMdl;
    /* 0x374 */ m3d::smdl_c mTimeMdl;
    /* 0x390 */ m3d::smdl_c mMdlArr0[2];
    /* 0x3C8 */ m3d::smdl_c mMdlArr1[2];
    /* 0x400 */ m3d::anmChr_c mAnmChr;
    /* 0x438 */ STATE_MGR_DECLARE(dAcOShutter_c);
    /* 0x474 */ dAcObjRef_c node_0x474;
    /* 0x480 */ dAcObjRef_c node_0x480;
    /* 0x48C */ dFlowMgr_c mFlowMgr;
    /* 0x548 */ ActorEventRelated mEventRelated;
    /* 0x598 */ u8 _0x598[0x5F8 - 0x598];
    /* 0x5F8 */ bool field_0x5F8[2];
    /* 0x5FA */ u8 _0x5FA[0x601 - 0x5FA];
    /* 0x601 */ bool field_0x601;
};

#endif
