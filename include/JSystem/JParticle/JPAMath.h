#ifndef JPAMATH_H
#define JPAMATH_H

#include "common.h"
#include "egg/math/eggVector.h"

void JPAGetDirMtx(EGG::Vector3f const& param_0, f32 (*param_1)[4]);
void JPAGetYZRotateMtx(s16 angleY, s16 angleZ, f32 (*param_2)[4]);
void JPAGetXYZRotateMtx(s16 x, s16 y, s16 z, Mtx dst);
void JPASetRMtxTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4], EGG::Vector3f *param_2);
f32 JPACalcKeyAnmValue(f32 param_0, u16 param_1, f32 const* param_2);
void JPASetRMtxSTVecfromMtx(f32 const (*param_0)[4], f32 (*param_1)[4],
EGG::Vector3f* param_2, EGG::Vector3f* param_3);

#endif /* JPAMATH_H */
