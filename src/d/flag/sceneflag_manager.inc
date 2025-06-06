#include "common.h"
#include "d/flag/sceneflag_manager.h"
#include "toBeSorted/file_manager.h"

SceneflagManager *SceneflagManager::sInstance = nullptr;
u16 SceneflagManager::sTempFlags[] = {};
u16 SceneflagManager::sSceneFlags[8] = {};
u16 SceneflagManager::sZoneFlags[0xFC] = {};

void SceneflagManager::init() {}
void SceneflagManager::setShouldCommit(u16 flag) {
    mShouldCommit = 1;
}
SceneflagManager::SceneflagManager()
    : mSceneflags(sSceneFlags, ARRAY_LENGTH(sSceneFlags)), mTempflags(sTempFlags, ARRAY_LENGTH(sTempFlags)),
      mZoneflags(sZoneFlags, ARRAY_LENGTH(sZoneFlags)) {
    mSceneIdx = 0xFFFF;
    mShouldCommit = 0;
}

s32 SceneflagManager::isNotTempOrZoneFlag(u16 flag) {
    return flag < TEMPFLAG_OFFSET;
}

s32 SceneflagManager::isZoneFlag(u32 flag) const  {
    if (ZONEFLAG_OFFSET <= flag && flag < 0x100) {
        return 1;
    } else {
        return 0;
    }
}
void SceneflagManager::updateFlagindex(u16 sceneIdx) {
    if (sceneIdx != mSceneIdx) {
        mSceneIdx = sceneIdx;
        mSceneflags.copyFromSaveFile2(FileManager::GetInstance()->getSceneFlagsConst() + sceneIdx * 8, 0, 8);
        unsetZoneAndTempflags();
    }
}
void SceneflagManager::copyFromSave(u32 sceneIdx) {
    u16 sceneIdx2 = sceneIdx;
    mSceneIdx = sceneIdx2;
    mSceneflags.copyFromSaveFile2(FileManager::GetInstance()->getSceneFlagsConst() + sceneIdx2 * 8, 0, 8);
    mTempflags.copyFromSaveFile(FileManager::GetInstance()->getTempFlagsConst(), 0, mTempflags.mCount);
    mZoneflags.copyFromSaveFile(FileManager::GetInstance()->getZoneFlagsConst(), 0, mZoneflags.mCount);
}
void SceneflagManager::unsetAllTempflags() {
    u16 flags[4];
    memset(flags, 0, sizeof(flags));
    FileManager::GetInstance()->setTempFlags(flags, 0, 4);
}
void SceneflagManager::zoneflagsResetAll() {
    u16 flags[0xFC];
    memset(flags, 0, sizeof(flags));
    FileManager::GetInstance()->setZoneFlags(flags, 0, 0xFC);
}
void SceneflagManager::zoneflagsResetForRoom(u16 roomId) {
    u16 flags[4];
    memset(flags, 0, sizeof(flags));
    // ? weird mask
    FileManager::GetInstance()->setZoneFlags(flags, (roomId & 0x3FF) * 4, 4);
}
void SceneflagManager::unsetZoneAndTempflags() {
    unsetAllTempflags();
    mTempflags.unsetAll();
    unsetAllZoneflags();
}
void SceneflagManager::unsetAllZoneflags() {
    zoneflagsResetAll();
    mZoneflags.unsetAll();
}
void SceneflagManager::unsetZoneflagsForRoom(u16 roomId) {
    zoneflagsResetForRoom(roomId);
    // ? weird mask
    mZoneflags.setAllToZero((roomId & 0x3FF) * 4, 4);
}
u16 SceneflagManager::getZoneflagSlot(u16 roomId, u16 flag) const  {
    return (flag + roomId * 0x40) >> 4 & 0xFFF;
}
u16 SceneflagManager::getSceneflagSlotGlobal(u16 sceneIdx, u16 flag) const {
    return (flag + sceneIdx * 0x80) >> 4 & 0xFFF;
}
u16 SceneflagManager::getSceneflagSlot(u16 flag) const {
    return flag >> 4 & 0xFFF;
}
u16 SceneflagManager::getTempflagSlot(u16 flag) const {
    return flag >> 4 & 0xFFF;
}
bool SceneflagManager::checkZoneFlag(u16 roomId, u16 flag) const {
    if (flag == 0xFF) {
        return false;
    } else {
        u16 zoneflag = flag - ZONEFLAG_OFFSET;
        u16 *pData = FileManager::GetInstance()->getZoneFlagsConst();
        u16 slot = getZoneflagSlot(roomId, zoneflag);
        return mFlagHelper.checkFlag(slot, zoneflag % 16, pData, 0xFC);
    }
}
bool SceneflagManager::checkUncommittedZoneflag(u16 roomId, u16 flag) {
    if (flag == 0xFF) {
        return false;
    } else {
        u16 *pData;
        u16 zoneflag = flag - ZONEFLAG_OFFSET;
        pData = mZoneflags.getFlagPtrUnchecked();
        u16 slot = getZoneflagSlot(roomId, zoneflag);
        return mFlagHelper.checkFlag(slot, zoneflag % 16, pData, mZoneflags.mCount);
    }
}
bool SceneflagManager::checkUncommittedTempOrSceneflag(u16 flag) {
    u16 *pData;
    if (flag >= TEMPFLAG_OFFSET) {
        u16 tempflag = flag - TEMPFLAG_OFFSET;
        pData = mTempflags.getFlagPtrUnchecked();
        u16 slot = getTempflagSlot(tempflag);
        return mFlagHelper.checkFlag(slot, tempflag % 16, pData, mTempflags.mCount);
    } else {
        pData = mSceneflags.getFlagPtrUnchecked();
        return mFlagHelper.checkFlag(getSceneflagSlot(flag), flag % 16, pData, mSceneflags.mCount);
    }
}
u16 SceneflagManager::checkFlag(u16 roomId, u16 flag) const {
    if (isZoneFlag(flag)) {
        return checkZoneFlag(roomId, flag);
    } else {
        return checkTempOrSceneflag(flag);
    }
}
u16 SceneflagManager::checkSceneflagGlobal(u16 sceneIdx, u16 flag) const {
    u16 *pData = FileManager::GetInstance()->getSceneFlagsConst();
    return mFlagHelper.checkFlag(getSceneflagSlotGlobal(sceneIdx, flag), flag % 16, pData, 0x800);
}
u16 SceneflagManager::checkTempOrSceneflag(u16 flag) const {
    if (flag >= TEMPFLAG_OFFSET) {
        u16 tempflag = flag - TEMPFLAG_OFFSET;
        const u16 *pData = FileManager::GetInstance()->getTempFlagsConst();
        u16 slot = getTempflagSlot(tempflag);
        return mFlagHelper.checkFlag(slot, tempflag % 16, pData, 4);
    } else {
        return checkSceneflagGlobal(mSceneIdx, flag);
    }
}
u16 SceneflagManager::checkUncommittedFlag(u16 roomId, u16 flag) {
    if (isZoneFlag(flag)) {
        return checkUncommittedZoneflag(roomId, flag);
    } else {
        return checkUncommittedTempOrSceneflag(flag);
    }
}
void SceneflagManager::setZoneflag(u16 roomId, u16 flag) {
    if (checkUncommittedZoneflag2(roomId, flag) != 1 && flag != 0xFF) {
        u16 *pData;
        u16 zoneflag = flag - ZONEFLAG_OFFSET;
        pData = mZoneflags.getFlagPtrChecked();
        u16 slot = getZoneflagSlot(roomId, zoneflag);
        mFlagHelper.setFlag(slot, zoneflag % 16, pData, mZoneflags.mCount);
        setShouldCommit(flag);
    }
}
void SceneflagManager::setFlag(u16 roomId, u16 flag) {
    if (isZoneFlag(flag)) {
        setZoneflag(roomId, flag);
    } else {
        setTempOrSceneflag(flag);
    }
}
void SceneflagManager::setSceneflagGlobal(u16 sceneIdx, u16 flag) {
    u16 slot = getSceneflagSlotGlobal(sceneIdx, flag);
    u16 *pData = FileManager::GetInstance()->getSceneFlagsConst();
    u16 pCurData = pData[slot];
    mFlagHelper.setFlag(0, flag % 16, &pCurData, 2);
    FileManager::GetInstance()->setSceneFlags(&pCurData, slot, 1);
    if (sceneIdx == mSceneIdx) {
        u16 *pData2 = mSceneflags.getFlagPtrChecked();
        mFlagHelper.setFlag(getSceneflagSlot(flag), flag % 16, pData2, mSceneflags.mCount);
    }
    setShouldCommit(flag);
}
void SceneflagManager::setTempOrSceneflag(u16 flag) {
    if (checkUncommittedTempOrSceneflag2(flag) != 1) {
        u16 *pData;
        if (flag >= TEMPFLAG_OFFSET) {
            u16 tempflag = flag - TEMPFLAG_OFFSET;
            pData = mTempflags.getFlagPtrChecked();
            mFlagHelper.setFlag(getTempflagSlot(tempflag), tempflag % 16, pData, mTempflags.mCount);
        } else {
            pData = mSceneflags.getFlagPtrChecked();
            mFlagHelper.setFlag(getSceneflagSlot(flag), flag % 16, pData, mSceneflags.mCount);
        }
        setShouldCommit(flag);
    }
}
void SceneflagManager::unsetZoneflag(u16 roomId, u16 flag) {
    if (checkUncommittedZoneflag2(roomId, flag) != 0 && flag != 0xFF) {
        u16 *pData;
        u16 zoneflag = flag - ZONEFLAG_OFFSET;
        pData = mZoneflags.getFlagPtrChecked();
        u16 slot = getZoneflagSlot(roomId, zoneflag);
        mFlagHelper.unsetFlag(slot, zoneflag % 16, pData, mZoneflags.mCount);
        setShouldCommit(flag);
    }
}
void SceneflagManager::unsetFlag(u16 roomId, u16 flag) {
    if (isZoneFlag(flag)) {
        unsetZoneflag(roomId, flag);
    } else {
        unsetTempOrSceneflag(flag);
    }
}
void SceneflagManager::unsetSceneflagGlobal(u16 sceneIdx, u16 flag) {
    u16 slot = getSceneflagSlotGlobal(sceneIdx, flag);
    u16 *pData = FileManager::GetInstance()->getSceneFlagsConst();
    u16 pCurData = pData[slot];
    mFlagHelper.unsetFlag(0, flag % 16, &pCurData, 2);
    FileManager::GetInstance()->setSceneFlags(&pCurData, slot, 1);
    if (sceneIdx == mSceneIdx) {
        u16 *pData2 = mSceneflags.getFlagPtrChecked();
        mFlagHelper.unsetFlag(getSceneflagSlot(flag), flag % 16, pData2, mSceneflags.mCount);
    }
    setShouldCommit(flag);
}
void SceneflagManager::unsetTempOrSceneflag(u16 flag) {
    if (checkUncommittedTempOrSceneflag2(flag) != 0) {
        u16 *pData;
        if (flag >= TEMPFLAG_OFFSET) {
            u16 tempflag = flag - TEMPFLAG_OFFSET;
            pData = mTempflags.getFlagPtrChecked();
            mFlagHelper.unsetFlag(getTempflagSlot(tempflag), tempflag % 16, pData, mTempflags.mCount);
        } else {
            pData = mSceneflags.getFlagPtrChecked();
            mFlagHelper.unsetFlag(getSceneflagSlot(flag), flag % 16, pData, mSceneflags.mCount);
        }
        setShouldCommit(flag);
    }
}
s32 SceneflagManager::commit() {
    if (mSceneIdx == 0xFFFF) {
        return 0;
    } else if (mShouldCommit) {
        FileManager::GetInstance()->setSceneFlags(mSceneflags.getFlagPtrUnchecked(), mSceneIdx * 8, 8);
        FileManager::GetInstance()->setTempFlags(mTempflags.getFlagPtrUnchecked(), 0, mTempflags.mCount);
        FileManager::GetInstance()->setZoneFlags(mZoneflags.getFlagPtrUnchecked(), 0, mZoneflags.mCount);
        mShouldCommit = false;
        return 1;
    }
    return 0;
}
