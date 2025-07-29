#ifndef LIGHT_H
#define LIGHT_H

#include "MD_Math.h"

typedef struct Light
{
    MD_Math::VECTOR3 Position;

    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Specular;
} Light;

typedef struct Light_Directional
{
    MD_Math::VECTOR3 Direction;

    MD_Math::VECTOR3 Ambient;
    MD_Math::VECTOR3 Diffuse;
    MD_Math::VECTOR3 Specular;
} Light_Directional;

#endif