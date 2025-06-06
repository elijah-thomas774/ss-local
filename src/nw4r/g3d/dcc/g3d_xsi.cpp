#include "nw4r/g3d.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
namespace {

void MakeTexSrtMtx_S(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = rSrt.Su;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = 0.0f;

    pMtx->_10 = 0.0f;
    pMtx->_11 = rSrt.Sv;
    pMtx->_12 = 0.0f;
    pMtx->_13 = 1.0f - rSrt.Sv;
}

void MakeTexSrtMtx_R(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    pMtx->_00 = cosR;
    pMtx->_01 = -sinR;
    pMtx->_02 = 0.0f;
    pMtx->_03 = sinR;

    pMtx->_10 = sinR;
    pMtx->_11 = cosR;
    pMtx->_12 = 0.0f;
    pMtx->_13 = 1.0f - cosR;
}

void MakeTexSrtMtx_T(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = 1.0f;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -rSrt.Tu;

    pMtx->_10 = 0.0f;
    pMtx->_11 = 1.0f;
    pMtx->_12 = 0.0f;
    pMtx->_13 = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    pMtx->_00 = sucr;
    pMtx->_01 = -susr;
    pMtx->_02 = 0.0f;
    pMtx->_03 = susr;

    pMtx->_10 = svsr;
    pMtx->_11 = svcr;
    pMtx->_12 = 0.0f;
    pMtx->_13 = -svcr + 1.0f;
}

void MakeTexSrtMtx_RT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    pMtx->_00 = cosR;
    pMtx->_01 = -sinR;
    pMtx->_02 = 0.0f;
    pMtx->_03 = (sinR - cosR * rSrt.Tu) - sinR * rSrt.Tv;

    pMtx->_10 = sinR;
    pMtx->_11 = cosR;
    pMtx->_12 = 0.0f;
    pMtx->_13 = (-cosR - sinR * rSrt.Tu) + cosR * rSrt.Tv + 1.0f;
}

void MakeTexSrtMtx_ST(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = rSrt.Su;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -rSrt.Su * rSrt.Tu;

    pMtx->_10 = 0.0f;
    pMtx->_11 = rSrt.Sv;
    pMtx->_12 = 0.0f;
    pMtx->_13 = rSrt.Sv * (rSrt.Tv - 1.0f) + 1.0f;
}

void MakeTexSrtMtx_SRT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    pMtx->_00 = sucr;
    pMtx->_01 = -susr;
    pMtx->_02 = 0.0f;
    pMtx->_03 = (susr - sucr * rSrt.Tu) - susr * rSrt.Tv;

    pMtx->_10 = svsr;
    pMtx->_11 = svcr;
    pMtx->_12 = 0.0f;
    pMtx->_13 = (-svcr - svsr * rSrt.Tu) + svcr * rSrt.Tv + 1.0f;
}

void ProductTexSrtMtx_S(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 *= rSrt.Su;
    pMtx->_01 *= rSrt.Su;
    pMtx->_02 *= rSrt.Su;
    pMtx->_03 *= rSrt.Su;

    pMtx->_10 *= rSrt.Sv;
    pMtx->_11 *= rSrt.Sv;
    pMtx->_12 *= rSrt.Sv;
    pMtx->_13 = rSrt.Sv * (pMtx->_13 - 1.0f) + 1.0f;
}

void ProductTexSrtMtx_R(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    _0x = cosR * pMtx->_00 - sinR * pMtx->_10;
    _1x = sinR * pMtx->_00 + cosR * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = cosR * pMtx->_01 - sinR * pMtx->_11;
    _1x = sinR * pMtx->_01 + cosR * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = cosR * pMtx->_02 - sinR * pMtx->_12;
    _1x = sinR * pMtx->_02 + cosR * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = cosR * pMtx->_03 - sinR * pMtx->_13 + sinR;
    _1x = sinR * pMtx->_03 + cosR * pMtx->_13 - cosR + 1.0f;
    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_T(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_03 -= rSrt.Tu;
    pMtx->_13 += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    _0x = sucr * pMtx->_00 - susr * pMtx->_10;
    _1x = svsr * pMtx->_00 + svcr * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = sucr * pMtx->_01 - susr * pMtx->_11;
    _1x = svsr * pMtx->_01 + svcr * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = sucr * pMtx->_02 - susr * pMtx->_12;
    _1x = svsr * pMtx->_02 + svcr * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = sucr * pMtx->_03 - susr * pMtx->_13 + susr;
    _1x = svsr * pMtx->_03 + svcr * pMtx->_13 - svcr + 1.0f;

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_RT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    _0x = cosR * pMtx->_00 - sinR * pMtx->_10;
    _1x = sinR * pMtx->_00 + cosR * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = cosR * pMtx->_01 - sinR * pMtx->_11;
    _1x = sinR * pMtx->_01 + cosR * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = cosR * pMtx->_02 - sinR * pMtx->_12;
    _1x = sinR * pMtx->_02 + cosR * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    // clang-format off
    _0x = cosR * (pMtx->_03 - rSrt.Tu)
        - sinR * (pMtx->_13 + rSrt.Tv)
        + sinR;

    _1x = sinR * (pMtx->_03 - rSrt.Tu)
        + cosR * (pMtx->_13 + rSrt.Tv)
        - cosR + 1.0f;
    // clang-format on

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_ST(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 *= rSrt.Su;
    pMtx->_01 *= rSrt.Su;
    pMtx->_02 *= rSrt.Su;
    pMtx->_03 = (pMtx->_03 - rSrt.Tu) * rSrt.Su;

    pMtx->_10 *= rSrt.Sv;
    pMtx->_11 *= rSrt.Sv;
    pMtx->_12 *= rSrt.Sv;
    pMtx->_13 = (pMtx->_13 + rSrt.Tv - 1.0f) * rSrt.Sv + 1.0f;
}

void ProductTexSrtMtx_SRT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    _0x = sucr * pMtx->_00 - susr * pMtx->_10;
    _1x = svsr * pMtx->_00 + svcr * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = sucr * pMtx->_01 - susr * pMtx->_11;
    _1x = svsr * pMtx->_01 + svcr * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = sucr * pMtx->_02 - susr * pMtx->_12;
    _1x = svsr * pMtx->_02 + svcr * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    // clang-format off
    _0x = sucr * (pMtx->_03 - rSrt.Tu)
        - susr * (pMtx->_13 + rSrt.Tv)
        + susr;

    _1x = svsr * (pMtx->_03 - rSrt.Tu)
        + svcr * (pMtx->_13 + rSrt.Tv)
        - svcr + 1.0f;
    // clang-format on

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

} // namespace

typedef void (*CalcTexMtxFunc)(math::MTX34 *pMtx, const TexSrt &rSrt);

bool CalcTexMtx_Xsi(math::MTX34 *pMtx, bool set, const TexSrt &rSrt, TexSrt::Flag flag) {
    // Extract S/R/T flags
    u32 index = flag >> 1 & 0b111;

    // Scale-one, no rotate/trans
    if (index == 0b111) {
        return false;
    }

    if (set) {
        static const CalcTexMtxFunc funcTable[] = {
            MakeTexSrtMtx_SRT, // 0 0 0
            MakeTexSrtMtx_RT,  // 0 0 1
            MakeTexSrtMtx_ST,  // 0 1 0
            MakeTexSrtMtx_T,   // 0 1 1
            MakeTexSrtMtx_SR,  // 1 0 0
            MakeTexSrtMtx_R,   // 1 0 1
            MakeTexSrtMtx_S    // 1 1 0
        };

        funcTable[index](pMtx, rSrt);
    } else {
        static const CalcTexMtxFunc funcTable[] = {
            ProductTexSrtMtx_SRT, // 0 0 0
            ProductTexSrtMtx_RT,  // 0 0 1
            ProductTexSrtMtx_ST,  // 0 1 0
            ProductTexSrtMtx_T,   // 0 1 1
            ProductTexSrtMtx_SR,  // 1 0 0
            ProductTexSrtMtx_R,   // 1 0 1
            ProductTexSrtMtx_S    // 1 1 0
        };

        funcTable[index](pMtx, rSrt);
    }

    pMtx->_20 = 0.0f;
    pMtx->_21 = 0.0f;
    pMtx->_22 = 1.0f;
    pMtx->_23 = 0.0f;

    return true;
}

u32 CalcWorldMtx_Xsi(
    math::MTX34 *pW, math::VEC3 *pS, const math::MTX34 *pW1, const math::VEC3 *pS1, u32 attr,
    const ChrAnmResult *pResult
) {
    u32 flag = pResult->flags;
    u32 newAttr = attr;

    if ((flag & ChrAnmResult::FLAG_MTX_IDENT) || (flag & ChrAnmResult::FLAG_ROT_TRANS_ZERO)) {
        math::MTX34Copy(pW, pW1);
    } else if (flag & ChrAnmResult::FLAG_ROT_ZERO) {
        if (detail::WorldMtxAttr::IsAllScaleOne(attr)) {
            math::VEC3 trans(pResult->rt._03, pResult->rt._13, pResult->rt._23);

            math::MTX34Trans(pW, pW1, &trans);
        } else {
            math::VEC3 trans(pS1->x * pResult->rt._03, pS1->y * pResult->rt._13, pS1->z * pResult->rt._23);

            math::MTX34Trans(pW, pW1, &trans);
        }
    } else if (detail::WorldMtxAttr::IsAllScaleOne(attr)) {
        math::MTX34Mult(pW, pW1, &pResult->rt);
    } else {
        math::MTX34 m;

        math::MTX34Copy(&m, &pResult->rt);

        m._03 *= pS1->x;
        m._13 *= pS1->y;
        m._23 *= pS1->z;

        math::MTX34Mult(pW, pW1, &m);
    }

    if (flag & ChrAnmResult::FLAG_SCALE_ONE) {
        newAttr = detail::WorldMtxAttr::AnmScaleOne(newAttr);
        *pS = *pS1;
    } else {
        newAttr = detail::WorldMtxAttr::AnmNotScaleOne(newAttr);
        pS->x = pS1->x * pResult->s.x;
        pS->y = pS1->y * pResult->s.y;
        pS->z = pS1->z * pResult->s.z;
    }

    if (flag & ChrAnmResult::FLAG_SCALE_UNIFORM) {
        newAttr = detail::WorldMtxAttr::AnmScaleUniform(newAttr);
    } else {
        newAttr = detail::WorldMtxAttr::AnmNotScaleUniform(newAttr);
    }

    return newAttr;
}

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r
