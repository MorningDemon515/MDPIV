#include "MD_Math.h"

typedef struct Light
{
    MD_Math::VECTOR3 position;

    MD_Math::VECTOR3 ambient;
    MD_Math::VECTOR3 diffuse;
    MD_Math::VECTOR3 specular;
} Light;
