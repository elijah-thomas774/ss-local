#ifndef M_MTX_H
#define M_MTX_H

// NOTE:: Comments about functions pulled from NSMBW

#include "common.h"
#include "egg/math/eggMatrix.h"
#include "m/m_angle.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"

class mMtx_c : public EGG::Matrix34f {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

public:
    mMtx_c() {};
    mMtx_c(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw);

    operator MtxRef() {
        return (MtxRef)(this);
    }
    operator MtxRefConst() const {
        return (MtxRefConst)(this);
    }
    operator nw4r::math::MTX34 *() {
        return (nw4r::math::MTX34 *)(this);
    }
    operator nw4r::math::MTX34 &() {
        return *(nw4r::math::MTX34 *)(this);
    }
    operator const nw4r::math::MTX34 *() const {
        return (nw4r::math::MTX34 *)(this);
    }
    operator const  nw4r::math::MTX34 &() const {
        return *(nw4r::math::MTX34 *)(this);
    }

    void XrotS(const mAng &angle); ///< Generates a rotation matrix for the X axis with the given angle.
    void XrotM(const mAng &angle); ///< Rotates the matrix on the X axis by the given angle.
    void YrotS(const mAng &angle); ///< Generates a rotation matrix for the Y axis with the given angle.
    void YrotM(const mAng &angle); ///< Rotates the matrix on the Y axis by the given angle.
    void ZrotS(const mAng &angle); ///< Generates a rotation matrix for the Z axis with the given angle.
    void ZrotM(const mAng &angle); ///< Rotates the matrix on the Z axis by the given angle.

    void ZXYrotS(
        const mAng &xRot, const mAng &yRot,
        const mAng &zRot
    ); ///< Generates the matrix on the Y, X and Z axes by the given angles.
    void ZXYrotM(
        const mAng &xRot, const mAng &yRot,
        const mAng &zRot
    ); ///< Rotates the matrix on the Y, X and Z axes by the given angles.
    void XYZrotS(
        const mAng &xRot, const mAng &yRot,
        const mAng &zRot
    ); ///< Generates the matrix on the Z, Y and X axes by the given angles.

    void XYZrotM(
        const mAng &xRot, const mAng &yRot,
        const mAng &zRot
    ); ///< Rotates the matrix on the Z, Y and X axes by the given angles.

    void ZYXrotM(
        const mAng &xRot, const mAng &yRot,
        const mAng &zRot
    ); ///< Rotates the matrix on the Z, Y and X axes by the given angles.

    void XYZrotM(const mAng3_c &ang) {
        XYZrotM(ang.x, ang.y, ang.z);
    }
    void ZXYrotM(const mAng3_c &ang) {
        ZXYrotM(ang.x, ang.y, ang.z);
    }
    void ZYXrotM(const mAng3_c &ang) {
        ZYXrotM(ang.x, ang.y, ang.z);
    }

    void toRot(mAng3_c &out) const; ///< Converts the matrix to a rotation vector.

    void multVecZero(nw4r::math::VEC3 &out) const; ///< Converts the matrix to a vector.
    void zero();                                   ///< Zeroes out the matrix.

    void rot(int, int); // does some werrd operation to rotate the matrix
    bool quatRelated();

    void transS(const mVec3_c &v) {
        PSMTXTrans(*this, v.x, v.y, v.z);
    }
    void transS(f32 x, f32 y, f32 z) {
        PSMTXTrans(*this, x, y, z);
    }
    mVec3_c multVec(const mVec3_c &v) const {
        mVec3_c ret = v;
        PSMTXMultVec(*this, ret, ret);
        return ret;
    }

    void multVec(const mVec3_c &in, mVec3_c &out) const {
        PSMTXMultVec(*this, in, out);
    }

    mMtx_c &operator+=(const mMtx_c &rhs) {
        PSMTXConcat(*this, rhs, *this);
        return *this;
    }

    mVec3_c operator*(const mVec3_c &rhs) {
        mVec3_c out;
        PSMTXMultVec(*this, rhs, out);
        return out;
    }

    void applyQuat(mQuat_c &quat) {
        PSMTXMultVec(m, quat.v, quat.v);
    }

    void fn_802F1C40(s32, s32);
    void makeRotationFromVecs(const mVec3_c &, const mVec3_c &, f32);

public:
    static mMtx_c Identity;
};

#endif
