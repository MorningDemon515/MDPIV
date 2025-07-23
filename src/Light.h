#include "MD_Math.h"

typedef struct Light
{
    MD_Math::VECTOR3 position;

    MD_Math::VECTOR3 ambient;
    MD_Math::VECTOR3 diffuse;
    MD_Math::VECTOR3 specular;
} Light;

typedef struct DirectionLight
{
    MD_Math::VECTOR3 direction;

    MD_Math::VECTOR3 ambient;
    MD_Math::VECTOR3 diffuse;
    MD_Math::VECTOR3 specular;
} DirectionLight;

typedef struct PointLight
{
    MD_Math::VECTOR3 position;  

    MD_Math::VECTOR3 ambient;
    MD_Math::VECTOR3 diffuse;
    MD_Math::VECTOR3 specular;

    float constant;
    float linear;
    float quadratic;
} PointLight;

typedef struct SpotLight
{
    MD_Math::VECTOR3  position;
    MD_Math::VECTOR3  direction;
    float cutOff;
    float outerCutOff;

    MD_Math::VECTOR3 ambient;
    MD_Math::VECTOR3 diffuse;
    MD_Math::VECTOR3 specular;
	
    float constant;
    float linear;
    float quadratic;

} SpotLight;
