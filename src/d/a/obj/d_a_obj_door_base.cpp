#include "d/a/obj/d_a_obj_door_base.h"

#include "common.h"
#include "d/d_stage.h"
#include "f/f_list_mg.h"
#include "f/f_list_nd.h"

fLiMgBa_c dAcObjDoor_c::sDoorList;

dAcObjDoor_c::dAcObjDoor_c() : mDoorRef(this) {
    sDoorList.append(&mDoorRef);
}

dAcObjDoor_c::~dAcObjDoor_c() {
    sDoorList.remove(&mDoorRef);
}

bool dAcObjDoor_c::hasVisitedRoom() {
    return dStage_c::GetInstance()->hasVisitedRoom(mRoomID);
}

void dAcObjDoor_c::vt_0x88() {
    // no-op
    return;
}

void dAcObjDoor_c::vt_0x8C() {
    // no-op
    return;
}

dAcObjDoor_c *dAcObjDoor_c::getNextDoorInList(dAcObjDoor_c *pDoor) {
    fLiNdBa_c *pNode = getNextNode(pDoor);
    if (pNode == nullptr) {
        return nullptr;
    }
    return static_cast<dAcObjDoor_c *>(pNode->p_owner);
}

fLiNdBa_c *dAcObjDoor_c::getNextNode(dAcObjDoor_c *pDoor) {
    if (pDoor == nullptr) {
        return sDoorList.getFirst();
    }
    return pDoor->mDoorRef.getNext();
}
