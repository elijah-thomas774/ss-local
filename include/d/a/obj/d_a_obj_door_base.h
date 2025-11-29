#ifndef D_A_OBJ_DOOR_BASE_H
#define D_A_OBJ_DOOR_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "f/f_list_mg.h"

class dAcObjDoor_c : public dAcObjBase_c {
public:
    dAcObjDoor_c();
    ~dAcObjDoor_c();

    virtual bool canOpen() = 0;
    virtual bool hasVisitedRoom();
    virtual void vt_0x88();
    virtual void vt_0x8C();

    static dAcObjDoor_c *getNextDoorInList(dAcObjDoor_c *);
    static fLiNdBa_c *getNextNode(dAcObjDoor_c *);

protected:
    fLiNdBa_c mDoorRef; ///< the node in the sDoorList. Needs to be the base based on destructor call

private:
    static fLiMgBa_c sDoorList;
};

#endif
