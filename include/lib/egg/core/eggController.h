#pragma once

#include <types.h>

#include <egg/math/eggMatrix.h>
#include <egg/math/eggVector.h>
#include <egg/prim/eggBitFlag.h>
#include <egg/prim/eggBuffer.h>
#include <rvl/PAD.h>

namespace EGG
{

class ControllerRumbleMgr;


// To Be Filled out
enum eCoreDevType {
};

class CoreStatus {
public:
    /* 0x00 */ u8 field_0x00[0xF0];
public:
    /* 80498f90 */ void init();
    /* 80498fa0 */ u32 getFSStickButton() const;
};

class CoreController {
public:
    // vtable 0x000 | 8056ec50
    /* vt 0x08 | 80064920 */ virtual void setPosParam(f32, f32);
    /* vt 0x0C | 8049a940 */ virtual void setHoriParam(f32, f32);
    /* vt 0x10 | 8049a930 */ virtual void setDistParam(f32, f32);
    /* vt 0x14 | 8049a920 */ virtual void setAccParam(f32, f32);
    /* vt 0x18 | 80059820 */ virtual bool isPressed(u32 mask);
    /* vt 0x1C | 80059a60 */ virtual bool isAnyPressed(u32 mask);
    /* vt 0x20 | 80014e30 */ virtual bool isTriggered(u32 mask);
    /* vt 0x24 | 800599e0 */ virtual bool isReleased();
    /* vt 0x28 | 80059840 */ virtual bool isAllPressed();
    /* vt 0x2C | 80059a80 */ virtual bool isNotPressed();
    /* vt 0x30 | 80499660 */ virtual void beginFrame(void* padStatus); // Really needs to be PADStatus
    /* vt 0x34 | 80499a60 */ virtual void endFrame();
public:
    /* 0x004 */ s32 mChannelID;
    /* 0x008 */ u32 mButtonHeld; // these fields may need a slight amount of work
    /* 0x00C */ u32 mButtonTriggered;
    /* 0x010 */ u32 mButtonReleased;
    /* 0x014 */ u32 field_0x14;
    /* 0x018 */ CoreStatus coreStatus[16];
    /* 0xf18 */ int mKPADResult;
    /* 0xf1c */ TBitFlag<u8> mFlag;
    /* 0xf20 */ Vector3f mAccel;
    /* 0xf2c */ Vector2f mDpdPos;
    /* 0xf34 */ u32 field_0xf28;
    /* 0xf38 */ u32 mMotorPattern;
    /* 0xf3c */ bool mEnableMotor;
    /* 0xf40 */ s32 mMotorFrameDuration;
    /* 0xf44 */ u8 mMotorPatternLength;
    /* 0xf45 */ u8 mMotorPatternPos;
    /* 0xf48 */ ControllerRumbleMgr* mRumbleMgr;
    /* 0xf4c */ Matrix34f mPostureMatrix;
    /* 0xf7c */ Matrix34f mPostureMatrixPrev;
    /* 0xfac */ TBitFlag<u8> mAccelFlags;
    /* 0xfb0 */ s32 mMaxAccelFrameTime;
    /* 0xfb4 */ s32 mAccelFrameTimeX;
    /* 0xfb8 */ s32 mAccelFrameTimeY;
    /* 0xfbc */ s32 mAccelFrameTimeZ;
    /* 0xfc0 */ f32 mMaxAccelDiff;
    /* 0xfc4 */ Vector3f mPrevAccel; 
public:
    /* 80499050 */ CoreController();
    /* 804990b0 */ void sceneReset();
    /* 80499170 */ Vector2f getDpdRawPos();
    /* 80499190 */ s32 getDpdDistance();
    /* 804991a0 */ void startMotor();
    /* 804991b0 */ void stopMotor();
    /* 804991c0 */ void createRumberMgr(u8);
    /* 80499220 */ void startPatternRumble(const char*, int, bool);
    /* 80499240 */ void stopRumbleMgr();
    /* 80499260 */ CoreStatus* getCoreStatus(s32 idx);
    /* 8056ebf8 */ void calc_posture_matrix(Matrix34f& mat, bool);
    /* 80499ac0 */ f32 getFreeStickX();
    /* 80499ae0 */ f32 getFreeStickY();
};

class NullController : public CoreController {
public:
    // idk this has NO effect on anything
    u8 unk[0x80674c00-0x80674b10];
};

class ControllerRumbleUnit {
public:
    // 0x00 vtable | 8056ebb4
    /* vt 0x08 | 8049a8e0 */ virtual ~ControllerRumbleUnit();
public:
    /* 0x04 */ char* mPattern;
    /* 0x08 */ char* mPatternPos;
    /* 0x0C */ s32   mTimer; // guess
    /* 0x10 */ f32   mRampUp; // guess
    /* 0x14 */ f32   mIntesity; // guess
    /* 0x18 */ TBitFlag<u8> mFlag;
    /* 0x1C */ nw4r::ut::Node mNode;
    /* 0x24 */ u32   field_0x24; // could just be part of the node (List)
public:
    /* 8049a3b0 */ void init();
    /* 8049a3e0 */ void startPattern(const char* patter, int);
    /* 8049a440 */ f32 calc();
};

class ControllerRumbleMgr {
public:
    // 0x00 vtable | 8056eba8
    /* vt 0x08 | 8049a8a0 */ virtual ~ControllerRumbleMgr();
public:
    /* 0x04 */ nw4r::ut::List mActiveUnitList;
    /* 0x10 */ nw4r::ut::List mInactiveUnitList;
    /* 0x1C */ CoreController* mController;
public:
    /* 8049a530 */ ControllerRumbleMgr();
    /* 8049a590 */ void createUnit(u8 numUnits, CoreController* controller);
    /* 8049a620 */ void stop();
    /* 8049a690 */ void calc();
    /* 8049a7a0 */ void startPattern(const char* pattern, int, bool);
    /* 8049a7f0 */ ControllerRumbleUnit* getUnitFromList(bool bGrabActive);
};
class CoreControllerMgr {
public:
    struct T__Disposer {
        Disposer mDisposer;
        /* vt 0x08 | 80499b00 */ virtual ~T__Disposer();
        /* 805767ac */ static T__Disposer* sStaticDisposer;
    };
    // Disposer Vtable: 8056ec40
    /* 0x0000 */ T__Disposer mDisposer; // for the static T__Disposer
public:
    // 0x0010 vtable | 8056ebf8
    /* vt 0x08 | 8049a130 */ void beginFrame();
    /* vt 0x0C | 8049a1e0 */ void endFrame();
public:
    /* 0x0014 */ TBuffer<CoreController*> mControllers;
    /* 0x0020 */ u8 field_0x20[0x10a4 - 0x0020];
    /* 0x10A4 */ TBuffer<eCoreDevType> mDevTypes;
    /* 0x10b0 */ u8 field_0x10B0[0x10e0 - 0x10b0];
public:
    /* 80499b80 */ static void createInstance();
    /* 80499bd0 */ static void deleteInstance();
    /* 80499cd0 */ void connectCallback(s32, s32);
    /* 80499d10 */ CoreControllerMgr();
public:
    /* 805767a8 */ static CoreControllerMgr* sInstance;
    /* 805767b0 */ static CoreController* (*sCoreControllerFactory)();
    /* 805767b4 */ static void (*sConnectCallback)(s32, s32); 
    /* 805767b8 */ static bool sUnk;
    // /* 805767bc */ static sAllocator; // defined in cpp file


// Other Stuff thats autogen (buffers/sinit/etc)
// /* 8049a950 */ void TBuffer<eCoreDevType>::allocate(int, int);
// /* 8049a9a0 */ void TBuffer<eCoreDevType>::allocate(int, Heap*, int);
// /* 8049aa20 */ void TBuffer<eCoreDevType>::onAllocate(Heap*);
// /* 8049aa30 */ void TBuffer<CoreController*>::allocate(int, int);
// /* 8049aa80 */ void TBuffer<CoreController*>::allocate(int, Heap*, int);
// /* 8049ab00 */ void TBuffer<CoreController*>::onAllocate(Heap*);
// /* 8049ab10 */ void sinit(); // NULL CONTROLLER

};

} // namespace EGG
