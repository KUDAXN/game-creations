#include <vector>
#include <math.h>
#include <DxLib.h>
#include "UshiUtility.h"

int UshiUtility::Round(float v)
{
    return static_cast<int>(roundf(v));
}

double UshiUtility::Rad2DegD(double rad)
{
    return rad * (180/DX_PI);
}

float UshiUtility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int UshiUtility::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double UshiUtility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180);
}

float UshiUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int UshiUtility::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

bool UshiUtility::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // ‹…‘Ì“¯m‚ÌÕ“Ë”»’è
    bool ret = false;

    // ‚¨Œİ‚¢‚Ì”¼Œa‚Ì‡Œv
    float radius = radius1 + radius2;

    // À•W‚Ì·‚©‚ç‚¨Œİ‚¢‚Ì‹——£‚ğæ‚é
    VECTOR diff = VSub(pos2, pos1);

    //O•½•û‚Ì’è—
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < radius * radius)
    {
        ret = true;
    }

    return ret;
}

bool UshiUtility::IsHitSphereCapsule(const VECTOR& sphPos, float sphRadius, const VECTOR& capPos1, const VECTOR& capPos2, float capRadius)
{
    return false;
}
