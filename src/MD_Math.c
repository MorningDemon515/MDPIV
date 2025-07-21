#ifdef __cplusplus
extern "C" {
#endif

#include "MD_Math.h"

const float MD_MATH_PI = 3.14159265358979323846f;//26433832795028841971693993751058209749445923078164062862089986280348253421170679f;
//Take 100 decimal places for pi

const float MD_MATH_E = 2.71828182845904523536f;//02874713526624977572470936999595749669676277240766303535475945713821785251664274f;

const float MD_MATH_PIReciprocal = 0.31830988618379067153f;//7767526745028724068919291480912897495334688117793595268453070180227605532506171912f;

const float MD_MATH_THIRD = 0.33333333333333333333f;//33333333333333333333333333333333333333333333333333333333333333333333333333333333;

//The value of root number 2 must be 100 digits
const float MD_MATH_SQRT2 = 1.41421356237309504880f;//1688724209698078569671875376948073176679737990732478462107038850387534327641573f;
//The value of root number 3 must be 100 digits
const float MD_MATH_SQRT3 = 1.73205080756887729352f;//7446341505872366942805253810380628055806979451933016908800037081146186757248576f;

const float MD_MATH_OneHundredAndEighty = 0.005555555555555556f;

const float MD_MATH_1Degree = 0.017453f;
const float MD_MATH_1Radian = 57.30f;

const float MD_MATH_EPSILON = 1.192092896e-07f;

const float MD_MATH_LN2 = 0.69314718055994530942f;

const MD_MATH_VECTOR2 BasisVector2I = { 1.0f,0.0f };
const MD_MATH_VECTOR2 BasisVector2J = { 0.0f,1.0f };

const MD_MATH_VECTOR3 BasisVector3I = { 1.0f,0.0f,0.0f };
const MD_MATH_VECTOR3 BasisVector3J = { 0.0f,1.0f,0.0f };

const MD_MATH_VECTOR4 BasisVector4I = { 1.0f,0.0f,0.0f,0.0f };
const MD_MATH_VECTOR4 BasisVector4J = { 0.0f,1.0f,0.0f,0.0f };

const MD_MATH_VECTOR3 BasisVector3K = { 0.0f,0.0f,1.0f };
const MD_MATH_VECTOR4 BasisVector4K = { 0.0f,0.0f,1.0f,0.0f };

const MD_MATH_VECTOR4 BasisVector4W = { 0.0f,0.0f,0.0f,1.0f };

const MD_MATH_MATRIX MD_Math_IdentityMatrix = {
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f
};

float MD_Math_Abs(float number)
{
    if(number > 0.0f)
    {
        return number;
    }
    else if (number == 0.0f)
    {
        return 0.0f;
    }
    else if (number < 0.0f)
    {
        return -1.0f * number;
    }
    else
    {
        return 0.0f;
    }
    
}

float MD_Math_Mod(float number1, float number2)
{
    if(number2 == 0.0f)
        return 0.0f;
    
    int y = (int)(number1 / number2);
    return number1 - (float)y * number2;
}


float MD_Math_Max(float number1, float number2)
{
    if(number1 > number2)
    {
        return number1;
    }
    else
    {
        return number2;
    }
    
}

float MD_Math_Min(float number1, float number2)
{
    if(number1 < number2)
    {
        return number1;
    }
    else
    {
        return number2;
    }
    
}

float MD_Math_Sqrt(float number)
{
    long i;
	float y;

	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = ( i >> 1 ) + 0x1fc00000;               // new magic number
	y  = * ( float * ) &i;
    y = 0.5f * ( y + number / y);// 1st iteration
    y = 0.5f * ( y + number / y);// 2nd iteration
    //y = 0.5f * ( y + number / y);// 3rd iteration

	return y;
}

float MD_Math_Cbrt(float number)
{
    long i;
	float y;

	y  = number;
	i  = * ( long * ) &y;					 // evil floating point bit level hacking
	i  = ( i / 3 ) + 0x2a9f84fe;               // new magic number
	y  = * ( float * ) &i;
    y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 1st iteration
    y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 2nd iteration
    y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 3rd iteration
    y = MD_MATH_THIRD * (2.0f * y + number / (y*y));// 4th iteration

	return y;
}

//From https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c#L552
float MD_Math_Rsqrt(float number)
{
    long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f375a86 - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float MD_Math_Hypot(float x,float y)
{
    return MD_Math_Sqrt((x*x + y*y));
}

float MD_Math_Factorial(int number)
{
    float r = 1.0f;
    
    float f= 1.0f;
    for (int i = 1; i < number; i++)
    {
        f = f + 1.0f;
        r = r * f;

    }

    return r;
    
}

float MD_Math_Pow(float x, int y)
{
    float r = 1.0f ;

    for (int i = 0; i < y; i++)
    {
        r = r * x;
    }
    
    return r;
}

float MD_Math_Sin(float x)
{
   float r = 0.0f;

   int k = (int)(x / MD_MATH_2PI);

   if(k < 0)
   { 
        for (int j = 0; j < (-1) * k; j++)
        {
            x = x + MD_MATH_2PI;
        }
    }
    
    for (int j = 0; j < k; j++)
    {
        x = x - MD_MATH_2PI;
    }

   for (int i = 0; i < 13; i++)
   {
    r = r +((MD_Math_Pow(-1.0f,i)/ MD_Math_Factorial((float)(2 * i + 1))) *
          MD_Math_Pow(x,(2 * i + 1)));
   }
   
   return r;
}

float MD_Math_Cos(float x) 
{
    float r = 0.0f;

   int k = (int)(x / MD_MATH_2PI);
    
    if(k < 0)
    {
        for (int j = 0; j < (-1) * k; j++)
        {
            x = x + MD_MATH_2PI;
        }
    }

    for (int j = 0; j < k; j++)
    {
         x = x - MD_MATH_2PI;
    }

   for (int i = 0; i < 13; i++)
   {
    r = r +((MD_Math_Pow(-1.0f,i)/ MD_Math_Factorial((float)(2 * i))) *
          MD_Math_Pow(x,(2 * i)));
   }
   
   return r;   
}

float MD_Math_Tan(float x )
{
    return MD_Math_Sin(x) / MD_Math_Cos(x);
}

float MD_Math_Cot(float x)
{
    return MD_Math_Cos(x)/MD_Math_Sin(x);
}

float MD_Math_ArcSin(float x)
{
    float r = 0.0f;

    for (int i = 0; i < 8; i++)
    {
        r = r + (
            (MD_Math_Factorial((float)(2 * i)))/(MD_Math_Pow(4.0f, i) * ((float)(2 * i + 1)) * (MD_Math_Pow(MD_Math_Factorial(i),2)))
        ) * 
        (MD_Math_Pow(x, 2 * i + 1));
    }
    
    return r;
}

float MD_Math_ArcCos(float x)
{
    return MD_MATH_PI * 0.5f - MD_Math_ArcSin(x);
}

float MD_Math_ArcTan(float x)
{
    float r = 0.0f;

    for (int i = 0; i < 24; i++)
    {
        r = r + (MD_Math_Pow(-1.0f, i) / (float)(2 * i + 1) ) * 
                (MD_Math_Pow(x, 2 * i + 1));
    }
    
    return r;
}

float MD_Math_Sec(float x)
{
    return 1.0f/ MD_Math_Cos(x);
}

float MD_Math_Csc(float x)
{
    return 1.0f / MD_Math_Sin(x);
}

bool MD_Math_Equal(float a,float b, float epsilon)
{
    if(MD_Math_Abs(a - b) < epsilon)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float MD_Math_EtoXPower(float x)
{
    return 1 + x + (x*x)/MD_Math_Factorial(2.0f)+
    (x*x*x)/MD_Math_Factorial(3.0f)+
    (x*x*x*x)/MD_Math_Factorial(4.0f)+
    (x*x*x*x*x)/MD_Math_Factorial(5.0f)+
    (x*x*x*x*x*x)/MD_Math_Factorial(6.0f)+
    (x*x*x*x*x*x*x)/MD_Math_Factorial(7.0f)+
    (x*x*x*x*x*x*x*x)/MD_Math_Factorial(8.0f)+
    (x*x*x*x*x*x*x*x*x)/MD_Math_Factorial(9.0f)+
    (x*x*x*x*x*x*x*x*x*x)/MD_Math_Factorial(10.0f)+
    (x*x*x*x*x*x*x*x*x*x*x)/MD_Math_Factorial(11.0f)+
    (x*x*x*x*x*x*x*x*x*x*x*x)/MD_Math_Factorial(12.0f);
}

float MD_Math_lnx(float x)
{
    if (x <= 0.0) {
        return 0.0; 
    }

    int k = 0;
    while (x > 2.0) {
        x /= 2.0;
        k++;
    }

    float t = x - 1.0f;  
    float result = 0.0;
    float term = t;     
    float sign = 1.0;  
    int n = 1;           

    while (n <= 13) {
        double current = sign * term / n;
        result += (float)current;

        term *= t;      
        sign = -sign; 
        n++;           
    }

    result += k * MD_MATH_LN2;

    return result;
    
}

float MD_Math_lgx(float x)
{
    return MD_Math_lnx(x) * 0.43429448190325176f;
}

float MD_Math_log2(float x)
{
    return MD_Math_lnx(x) * 1.4426950408889634f;
}

float MD_Math_AngularToRadian(float angle)
{
    return angle * MD_MATH_PI * MD_MATH_OneHundredAndEighty;
}

float MD_Math_RadianToangle(float Radian)
{
    return Radian * 180.0f * MD_MATH_PIReciprocal;
}

float MD_Math_ComputeRectanglePerimeter(float Length, float Width)
{
    return (Length + Width) * 2;
}

float MD_Math_ComputeSquarePerimeter(float Length)
{
    return Length * 4;
}

float MD_Math_ComputeRectangleArea(float Length, float Width)
{
    return Length * Width;
}

float MD_Math_ComputeSquareArea(float Length)
{
    return Length * Length;
}

float MD_Math_ComputeTriangleArea(float Bottom, float Height)
{
    return Bottom * Height * 0.5f; 
}

float MD_Math_ComputeParallelogramArea(float Bottom, float Height)
{
    return Bottom * Height;
}

float MD_Math_ComputeTrapezoidArea(float Top, float Bottom,float Height)
{
    return (Top + Bottom) * Height * 0.5f;
}

float MD_Math_ComputeCirclePerimeter(float radius)
{
    return 2.0f * MD_MATH_PI * radius;
}

float MD_Math_ComputeCircleArea(float radius)
{
    return MD_MATH_PI * radius * radius;
}

float MD_Math_ComputeCuboidVolume(float Length, float Width, float Height)
{
    return Length * Width * Height;
}

float MD_Math_ComputeCubeVolume(float Length)
{
    return Length * Length;
}

float MD_Math_ComputeCylinderSideArea(float radius, float height)
{
    return 2.0f * MD_MATH_PI * radius * height;
}

float MD_Math_ComputeConeSideArea(float radius, float busbar)
{
    return 2.0f * MD_MATH_PI * radius * busbar;
}

float MD_Math_ComputeRoundTableSideArea(float radius1,float radius2,float busbar)
{
    return MD_MATH_PI * (radius1 + radius2) * busbar;
}

float MD_Math_ComputeCylinderSurfaceArea(float radius, float height)
{
    return MD_Math_ComputeCylinderSideArea(radius,height) + MD_Math_ComputeCircleArea(radius);
}

float MD_Math_ComputeConeSurfaceArea(float radius, float busbar)
{
    return MD_Math_ComputeCircleArea(radius) + MD_MATH_PI * radius * busbar;
}

float MD_Math_ComputeRoundTableSurfaceArea(float radius1,float radius2, float busbar)
{
    return MD_Math_ComputeCircleArea(radius1) + MD_Math_ComputeCircleArea(radius2) + MD_MATH_PI * radius1 * busbar + MD_MATH_PI * radius2 * busbar;
}

float MD_Math_ComputeBallSurfaceArea(float radius)
{
    return 4.0f * MD_MATH_PI * radius;
}

float MD_Math_ComputeCylinderVolume(float radius, float height)
{
    return MD_Math_ComputeCircleArea(radius) * height;
}

float MD_Math_ComputeConeVolume(float radius, float height)
{
    return MD_MATH_THIRD * MD_Math_ComputeCylinderVolume(radius,height);
}

float MD_Math_ComputeRoundTableVolume(float radius1, float radius2, float height)
{
    return MD_MATH_THIRD * height * (MD_Math_ComputeCircleArea(radius1) + MD_Math_ComputeCircleArea(radius2) + MD_MATH_PI * (radius1 + radius2));
}

float MD_Math_ComputeBallVolume(float radius)
{
    return 4.0f * MD_MATH_THIRD * MD_MATH_PI * radius * radius * radius;
}

// About Vector--------------------------------------------------------------------------------
bool MD_Math_Vector2Equal(MD_MATH_VECTOR2 v1,MD_MATH_VECTOR2 v2)
{
    if(
        MD_Math_Equal(v1.x,v2.x,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.y,v2.y,MD_MATH_EPSILON)
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MD_Math_Vector3Equal(MD_MATH_VECTOR3 v1,MD_MATH_VECTOR3 v2)
{
    if(
        MD_Math_Equal(v1.x,v2.x,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.y,v2.y,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.z,v2.z,MD_MATH_EPSILON) 
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MD_Math_Vector4Equal(MD_MATH_VECTOR4 v1,MD_MATH_VECTOR4 v2)
{
    if(
        MD_Math_Equal(v1.x,v2.x,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.y,v2.y,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.z,v2.z,MD_MATH_EPSILON) &&
        MD_Math_Equal(v1.w,v2.w,MD_MATH_EPSILON)
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

MD_MATH_VECTOR2 MD_Math_Vector2Addition(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2)
{
    MD_MATH_VECTOR2 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

MD_MATH_VECTOR3 MD_Math_Vector3Addition(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2)
{
    MD_MATH_VECTOR3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

MD_MATH_VECTOR4 MD_Math_Vector4Addition(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2)
{
    MD_MATH_VECTOR4 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;
    return result;
}

MD_MATH_VECTOR2 MD_Math_Vector2Multiplication(MD_MATH_VECTOR2 v,float x)
{
    MD_MATH_VECTOR2 r;
    r.x = x * v.x;
    r.y = x * v.y;
    return r;
}

MD_MATH_VECTOR3 MD_Math_Vector3Multiplication(MD_MATH_VECTOR3 v,float x)
{
    MD_MATH_VECTOR3 r;
    r.x = x * v.x;
    r.y = x * v.y;
    r.z = x * v.z;
    return r;
}

MD_MATH_VECTOR4 MD_Math_Vector4Multiplication(MD_MATH_VECTOR4 v,float x)
{
    MD_MATH_VECTOR4 r;
    r.x = x * v.x;
    r.y = x * v.y;
    r.z = x * v.z;
    r.w = x * v.w;
    return r;
}

float MD_Math_ComputeVector2Length(MD_MATH_VECTOR2 v)
{
    return MD_Math_Hypot(v.x,v.y);
}

float MD_Math_ComputeVector3Length(MD_MATH_VECTOR3 v)
{
    return MD_Math_Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float MD_Math_ComputeVector4Length(MD_MATH_VECTOR4 v)
{
    return MD_Math_Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

MD_MATH_VECTOR2 MD_Math_Vector2Normalized(MD_MATH_VECTOR2 v)
{
    MD_MATH_VECTOR2 r;
    r.x = v.x * MD_Math_Rsqrt(v.x*v.x + v.y * v.y); 
    r.y = v.y * MD_Math_Rsqrt(v.x*v.x + v.y * v.y); 
    return r;
}

MD_MATH_VECTOR3 MD_Math_Vector3Normalized(MD_MATH_VECTOR3 v)
{
    MD_MATH_VECTOR3 r;
    r.x = v.x * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
    r.y = v.y * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
    r.z = v.z * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z); 
    return r;
}

MD_MATH_VECTOR4 MD_Math_Vector4Normalized(MD_MATH_VECTOR4 v)
{
    MD_MATH_VECTOR4 r;
    r.x = v.x * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
    r.y = v.y * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    r.z = v.z * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
    r.w = v.w * MD_Math_Rsqrt(v.x*v.x + v.y * v.y + v.z * v.z + v.w * v.w);  
    return r;
}

float MD_Math_Vector2Dot(MD_MATH_VECTOR2 v1,MD_MATH_VECTOR2 v2)
{
    return 
       v1.x * v2.x +
       v1.y * v2.y
    ;
}

float MD_Math_Vector3Dot(MD_MATH_VECTOR3 v1,MD_MATH_VECTOR3 v2)
{
    return 
       v1.x * v2.x +
       v1.y * v2.y +
       v1.z * v2.z
    ;
}

float MD_Math_Vector4Dot(MD_MATH_VECTOR4 v1,MD_MATH_VECTOR4 v2)
{
    return 
       v1.x * v2.x +
       v1.y * v2.y +
       v1.z * v2.z +
       v1.w * v2.w
    ;
}

MD_MATH_VECTOR3 MD_Math_VectorCross(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2)
{
    MD_MATH_VECTOR3 r;
    r.x = v1.y * v2.z - v2.y * v1.z;
    r.y = v1.z * v2.x - v2.z * v1.x;
    r.z = v1.x * v2.y - v2.x * v1.y;
    return r;
}

float MD_Math_ProjectionOfVector2(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2)
{
    return 
        MD_Math_ComputeVector2Length(v1) * 
        MD_Math_Cos(MD_Math_ArcCos(MD_Math_Vector2Dot(v1,v2) * MD_Math_Rsqrt(v1.x * v1.x + v1.y * v1.y)*
                                                                MD_Math_Rsqrt(v2.x* v2.x + v2.y*v2.y )))
    ;
}

float MD_Math_ProjectionOfVector3(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2)
{
    return 
        MD_Math_ComputeVector3Length(v1) * 
        MD_Math_Cos(MD_Math_ArcCos(MD_Math_Vector3Dot(v1,v2) * MD_Math_Rsqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z)*
                                                                MD_Math_Rsqrt(v2.x* v2.x + v2.y*v2.y + v2.z * v2.z)))
    ;
}

float MD_Math_ProjectionOfVector4(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2)
{
    return 
        MD_Math_ComputeVector4Length(v1) * 
        MD_Math_Cos(MD_Math_ArcCos(MD_Math_Vector4Dot(v1,v2) * MD_Math_Rsqrt(v1.x * v1.x + v1.y * v1.y+ v1.z * v1.z+ v1.w * v1.w)*
                                                                MD_Math_Rsqrt(v2.x* v2.x + v2.y*v2.y + v1.z * v1.z+ v1.w * v1.w)))
    ;
}

MD_MATH_VECTOR2 MD_Math_ProjectionVector2(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2)
{
    MD_MATH_VECTOR2 r = MD_Math_Vector2Normalized(v2);
    return MD_Math_Vector2Multiplication(r, MD_Math_ProjectionOfVector2(v1,v2));
}

MD_MATH_VECTOR3 MD_Math_ProjectionVector3(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2)
{
    MD_MATH_VECTOR3 r = MD_Math_Vector3Normalized(v2);
    return MD_Math_Vector3Multiplication(r, MD_Math_ProjectionOfVector3(v1,v2));
}

MD_MATH_VECTOR4 MD_Math_ProjectionVector4(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2)
{
    MD_MATH_VECTOR4 r = MD_Math_Vector4Normalized(v2);
    return MD_Math_Vector4Multiplication(r, MD_Math_ProjectionOfVector4(v1,v2));
}
//---------------------------------------------------------------------------------------------------------
//About Matrix --------------------------------------------------------------------------------------------

MD_MATH_VECTOR4 MD_Math_VectorMulMatrix(MD_MATH_MATRIX m, MD_MATH_VECTOR4 v)
{
   MD_MATH_VECTOR4 r;
   r.x = v.x * m._11 + v.y * m._12 + v.z * m._13 + v.w * m._14;
   r.y = v.x * m._21 + v.y * m._22 + v.z * m._23 + v.w * m._24;
   r.z = v.x * m._31 + v.y * m._32 + v.z * m._33 + v.w * m._34;
   r.w = v.x * m._41 + v.y * m._42 + v.z * m._43 + v.w * m._44;
   return r;
}

MD_MATH_MATRIX MD_Math_MatrixMulMatrix(MD_MATH_MATRIX m1,MD_MATH_MATRIX m2)
{
    MD_MATH_MATRIX r;   
    MD_MATH_VECTOR4 v1 ,v2 ,v3,v4,
                    r1,r2,r3,r4;

    v1.x = m2._11; v1.y = m2._21; v1.z = m2._31; v1.w = m2._41;
    r1 = MD_Math_VectorMulMatrix(m1,v1);

    r._11  = r1.x;
    r._21  = r1.y;
    r._31  = r1.z;
    r._41  = r1.w;
    
    v2.x = m2._12; v2.y = m2._22; v2.z = m2._32; v2.w = m2._42;
    r2 = MD_Math_VectorMulMatrix(m1,v2);

    r._12  = r2.x;
    r._22  = r2.y;
    r._32  = r2.z;
    r._42  = r2.w;
    
    v3.x = m2._13; v3.y = m2._23; v3.z = m2._33; v3.w = m2._43;
    r3 = MD_Math_VectorMulMatrix(m1,v3);

    r._13  = r3.x;
    r._23  = r3.y;
    r._33  = r3.z;
    r._43  = r3.w;
    
    v4.x = m2._14; v4.y = m2._24; v4.z = m2._34; v4.w = m2._44;
    r4 = MD_Math_VectorMulMatrix(m1,v4);

    r._14  = r4.x;
    r._24  = r4.y;
    r._34  = r4.z;
    r._44  = r4.w;

    return r;

}


MD_MATH_MATRIX MD_Math_MatrixAddMatrix(MD_MATH_MATRIX m1,MD_MATH_MATRIX m2)
{
    MD_MATH_MATRIX r;
    r._11 = m1._11 + m2._11;
    r._21 = m1._21 + m2._21;
    r._31 = m1._31 + m2._31;
    r._41 = m1._41 + m2._41;

    r._12 = m1._12 + m2._12;
    r._22 = m1._22 + m2._22;
    r._32 = m1._32 + m2._32;
    r._42 = m1._42 + m2._42;
    
    r._13 = m1._13 + m2._13;
    r._23 = m1._23 + m2._23;
    r._33 = m1._33 + m2._33;
    r._43 = m1._43 + m2._43;

    r._14 = m1._14 + m2._14;
    r._24 = m1._24 + m2._24;
    r._34 = m1._34 + m2._34;
    r._44 = m1._44 + m2._44;

    return r;
}

MD_MATH_MATRIX MD_Math_MatrixMultiplication(MD_MATH_MATRIX m,float x)
{
    MD_MATH_MATRIX r;
    r._11 = x * m._11; 
    r._21 = x * m._21; 
    r._31 = x * m._31; 
    r._41 = x * m._41; 

    r._12 = x * m._12; 
    r._22 = x * m._22; 
    r._32 = x * m._32; 
    r._42 = x * m._42; 

    r._13 = x * m._13; 
    r._23 = x * m._23; 
    r._33 = x * m._33; 
    r._43 = x * m._43; 

    r._14 = x * m._14; 
    r._24 = x * m._24; 
    r._34 = x * m._34; 
    r._44 = x * m._44; 

    return r;
}

MD_MATH_MATRIX MD_Math_MatrixTranspose(MD_MATH_MATRIX m)
{
    MD_MATH_MATRIX r;

    r._11 = m._11;
    r._21 = m._12;
    r._31 = m._13;
    r._41 = m._14;

    r._12 = m._21;
    r._22 = m._22;
    r._32 = m._23;
    r._42 = m._24;

    r._13 = m._31;
    r._23 = m._32;
    r._33 = m._33;
    r._43 = m._34;

    r._14 = m._41;
    r._24 = m._42;
    r._34 = m._43;
    r._44 = m._44;
    
    return r;
}

float MD_Math_DetMatrix2x2(MD_MATH_MATRIX2x2 m)
{
    return m._11 * m._22 - m._12 * m._21;
}

float MD_Math_DetMatrix3x3(MD_MATH_MATRIX3x3 m)
{
    MD_MATH_MATRIX2x2 m1 = {m._22,m._23,
                            m._32,m._33};

    MD_MATH_MATRIX2x2 m2 = {m._21,m._23,
                            m._31,m._33};

    MD_MATH_MATRIX2x2 m3 = {m._21,m._22,
                            m._31,m._32};

    return m._11 * MD_Math_DetMatrix2x2(m1) - m._12 * MD_Math_DetMatrix2x2(m2) + m._13 * MD_Math_DetMatrix2x2(m3);  
}

float MD_Math_DetMatrix4x4(MD_MATH_MATRIX m)
{
    MD_MATH_MATRIX3x3 m1 = {m._22,m._23,m._24,
                            m._32,m._33,m._34,
                            m._42,m._43,m._44};       

    MD_MATH_MATRIX3x3 m2 = {m._21,m._23,m._24,
                            m._31,m._33,m._34,
                            m._41,m._43,m._44};

    MD_MATH_MATRIX3x3 m3 = {m._21,m._22,m._24,
                            m._31,m._32,m._34,
                            m._41,m._42,m._44};      

    MD_MATH_MATRIX3x3 m4 = {m._21,m._22,m._23,
                            m._31,m._32,m._33,
                            m._41,m._42,m._43};

    return m._11 * MD_Math_DetMatrix3x3(m1) - m._12 * MD_Math_DetMatrix3x3(m2) + m._13 * MD_Math_DetMatrix3x3(m3) - m._14 *MD_Math_DetMatrix3x3(m4);                                                                                                                                                
}

MD_MATH_MATRIX MD_Math_AdjointMatrix(MD_MATH_MATRIX m)
{
    MD_MATH_MATRIX rm;
    MD_MATH_MATRIX adj;      
 
    MD_MATH_MATRIX3x3 m1 = {m._22, m._23,m._24,
                            m._32,m._33,m._34,
                            m._42,m._43,m._44};
    MD_MATH_MATRIX3x3 m2 = {m._21,m._23,m._24,
                            m._31,m._33,m._34,
                            m._41,m._43,m._44};
    MD_MATH_MATRIX3x3 m3 = {m._21,m._22,m._24,
                            m._31,m._32,m._34,
                            m._41,m._42,m._44};
    MD_MATH_MATRIX3x3 m4 = {m._21,m._22,m._23,
                            m._31,m._32,m._33,
                            m._41,m._42,m._43};

    MD_MATH_MATRIX3x3 m5 = {m._12,m._13,m._14,
                            m._32,m._33,m._34,
                            m._42,m._43,m._44};
    MD_MATH_MATRIX3x3 m6 = {m._11,m._13,m._14,
                            m._31,m._33,m._34,
                            m._41,m._43,m._44};
    MD_MATH_MATRIX3x3 m7 = {m._11,m._12,m._14,
                            m._31,m._32,m._34,
                            m._41,m._42,m._44};
    MD_MATH_MATRIX3x3 m8 = {m._11,m._12,m._13,
                            m._31,m._32,m._33,
                            m._41,m._42,m._43};

    MD_MATH_MATRIX3x3 m9 = {m._12,m._13,m._14,
                            m._22,m._23,m._24,
                            m._42,m._43,m._44};
    MD_MATH_MATRIX3x3 m10 = {m._12,m._13,m._14,
                             m._21,m._23,m._24,
                             m._41,m._43,m._44};
    MD_MATH_MATRIX3x3 m11 = {m._11,m._12,m._14,
                             m._21,m._22,m._24,
                             m._41,m._42,m._44};
    MD_MATH_MATRIX3x3 m12 = {m._11,m._12,m._13,
                             m._21,m._22,m._23,
                             m._41,m._42,m._43};

    MD_MATH_MATRIX3x3 m13 = {m._12,m._13,m._14,
                             m._22,m._23,m._24,
                             m._32,m._33,m._34};
    MD_MATH_MATRIX3x3 m14 = {m._11,m._13,m._14,
                             m._21,m._23,m._24,
                             m._31,m._33,m._34};
    MD_MATH_MATRIX3x3 m15 = {m._11,m._12,m._14,
                             m._21,m._22,m._24,
                             m._31,m._32,m._34};
    MD_MATH_MATRIX3x3 m16 = {m._11,m._12,m._13,
                             m._21,m._22,m._23,
                             m._31,m._32,m._33};

    rm._11 = MD_Math_DetMatrix3x3(m1);
    rm._12 = (-1) * MD_Math_DetMatrix3x3(m2);
    rm._13 = MD_Math_DetMatrix3x3(m3);
    rm._14 = (-1) * MD_Math_DetMatrix3x3(m4);

    rm._21 = (-1) * MD_Math_DetMatrix3x3(m5);
    rm._22 = MD_Math_DetMatrix3x3(m6);
    rm._23 = (-1) * MD_Math_DetMatrix3x3(m7);
    rm._24 = MD_Math_DetMatrix3x3(m8);

    rm._31 = MD_Math_DetMatrix3x3(m9);
    rm._32 = (-1) * MD_Math_DetMatrix3x3(m10);
    rm._33 = MD_Math_DetMatrix3x3(m11);
    rm._34 = (-1) * MD_Math_DetMatrix3x3(m12);

    rm._41 = (-1) * MD_Math_DetMatrix3x3(m13);
    rm._42 = MD_Math_DetMatrix3x3(m14);
    rm._43 = (-1) * MD_Math_DetMatrix3x3(m15);
    rm._44 = MD_Math_DetMatrix3x3(m16);

    adj._11 = rm._11;
    adj._12 = rm._21;
    adj._13 = rm._31;
    adj._14 = rm._41;

    adj._21 = rm._12;
    adj._22 = rm._22;
    adj._23 = rm._32;
    adj._24 = rm._42;

    adj._31 = rm._13;
    adj._32 = rm._23;
    adj._33 = rm._33;
    adj._34 = rm._43;

    adj._41 = rm._14;
    adj._42 = rm._24;
    adj._43 = rm._34;
    adj._44 = rm._44;

    return adj;
}

MD_MATH_MATRIX MD_Math_InvMatrix(MD_MATH_MATRIX m)
{
   MD_MATH_MATRIX Inv,adj;
   
   float det = MD_Math_DetMatrix4x4(m);

   if(MD_Math_Abs(det) < MD_MATH_EPSILON)
   {
    return MD_Math_IdentityMatrix;
   }

   float _1det = 1.0f / det;
   adj = MD_Math_AdjointMatrix(m);

   Inv = MD_Math_MatrixMultiplication(adj,_1det);
    
    return Inv;
}

bool MD_Math_MatrixEqual(MD_MATH_MATRIX m1 , MD_MATH_MATRIX m2)
{
    if(
        MD_Math_Equal(m1._11,m2._11,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._21,m2._21,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._31,m2._31,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._41,m2._41,MD_MATH_EPSILON) &&

        MD_Math_Equal(m1._12,m2._12,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._22,m2._22,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._32,m2._32,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._42,m2._42,MD_MATH_EPSILON) &&

        MD_Math_Equal(m1._13,m2._13,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._23,m2._23,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._33,m2._33,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._43,m2._43,MD_MATH_EPSILON) &&

        MD_Math_Equal(m1._14,m2._14,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._24,m2._24,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._34,m2._34,MD_MATH_EPSILON) &&
        MD_Math_Equal(m1._44,m2._44,MD_MATH_EPSILON) 
    )
    {
        return true;
    }
    else{
        return false;
    }
}

void MD_Math_MatrixToValue(MD_MATH_MATRIX m, float* out) {
	out[0] = m._11; out[1] = m._21; out[2] = m._31; out[3] = m._41;
	out[4] = m._12; out[5] = m._22; out[6] = m._32; out[7] = m._42;
	out[8] = m._13; out[9] = m._23; out[10] = m._33; out[11] = m._43;
	out[12] = m._14; out[13] = m._24; out[14] = m._34; out[15] = m._44;
}

//Plane-------------------------------------------------------------------------------

MD_MATH_PLANE MD_Math_PlaneFromPointAndNormal(MD_MATH_VECTOR3 Point,MD_MATH_VECTOR3 Normal)
{
    MD_MATH_PLANE plane;
    plane.a = Point.x;
    plane.b = Point.y;
    plane.c = Point.z;
    plane.d = (-1.0f) * MD_Math_Vector3Dot(Normal,Point);

    return plane;
}

MD_MATH_PLANE MD_Math_PlaneFromPoints(MD_MATH_VECTOR3 p0,
                                      MD_MATH_VECTOR3 p1,
                                      MD_MATH_VECTOR3 p2)
{
    MD_MATH_PLANE plane;
    MD_MATH_VECTOR3 u = MD_Math_Vector3Addition(p1,MD_Math_Vector3Multiplication(p0,(-1.0f)));
    MD_MATH_VECTOR3 v = MD_Math_Vector3Addition(p2,MD_Math_Vector3Multiplication(p0,(-1.0f)));
    MD_MATH_VECTOR3 n = MD_Math_VectorCross(u,v);

    plane.a = n.x;
    plane.b = n.y;
    plane.c = n.z;
    plane.d = (-1.0f) * MD_Math_Vector3Dot(n,p0);

    return plane;
}

float MD_Math_PlaneDot(MD_MATH_PLANE p, MD_MATH_VECTOR3 v)
{
    MD_MATH_VECTOR3 n;
    float r;
    n.x = p.a;
    n.y = p.b;
    n.y = p.c;

    r = MD_Math_Vector3Dot(n,v) + p.d;
    return r;
}

MD_MATH_PLANE MD_Math_PlaneNormalize(MD_MATH_PLANE p)
{
    MD_MATH_PLANE r;
    float nn;

    nn = MD_Math_Rsqrt(p.a * p.a + p.b * p.b + p.c * p.c);

    r.a = nn * p.a;
    r.b = nn * p.b;
    r.c = nn * p.c;
    r.d = nn * p.d;

    return r;
    
}

MD_MATH_PLANE MD_Math_PlaneTransform(MD_MATH_PLANE p, MD_MATH_MATRIX m)
{
    MD_MATH_PLANE r;
    MD_MATH_VECTOR4 pp;
    p = MD_Math_PlaneNormalize(p);

    pp.x = p.a;
    pp.y = p.b;
    pp.z = p.c;
    pp.w = p.d;

    m = MD_Math_InvMatrix(m);
    m = MD_Math_MatrixTranspose(m);

    pp = MD_Math_VectorMulMatrix(m,pp);

    r.a = pp.x;
    r.b = pp.y;
    r.c = pp.z;
    r.d = pp.w;

    return r;
}

//Ray-----------------------------------------------------------------

MD_MATH_VECTOR3 MD_Math_CreateRay(MD_MATH_RAY ray,float t)
{
    MD_MATH_VECTOR3 r;

    if(t < 0.0f)
    {
        r.x = 0.0f;
        r.y = 0.0f;
        r.z = 0.0f;
        return r;
    }

    r = MD_Math_Vector3Addition(ray.p0,MD_Math_Vector3Multiplication(ray.u,t));

    return r;

}

MD_MATH_VECTOR3 MD_Math_Intersection(MD_MATH_RAY ray, MD_MATH_PLANE p)
{
    MD_MATH_VECTOR3 r;
    MD_MATH_VECTOR3 n;
    n.x = p.a;
    n.y = p.b;
    n.z = p.c;
    float t;

    t = ((-1) * p.d - MD_Math_Vector3Dot(n,ray.p0))/(MD_Math_Vector3Dot(n,ray.u));

    r = MD_Math_Vector3Addition(ray.p0,MD_Math_Vector3Multiplication(ray.u,t));

    return r;
}

MD_MATH_MATRIX MD_Math_ScaleMatrix(float x, float y, float z)
{
    MD_MATH_MATRIX r = {
        x,0.0f,0.0f,0.0f,
        0.0f,y,0.0f,0.0f,
        0.0f,0.0f,z,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };

    return r;
}

MD_MATH_MATRIX MD_Math_TranslationMatrix(float x, float y, float z)
{
    MD_MATH_MATRIX r = {
        1.0f, 0.0f ,0.0f ,x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f ,z,
        0.0f, 0.0f, 0.0f ,1.0f
    };

    return r;
}

MD_MATH_MATRIX MD_Math_RotationMatrix(float Angle, char Axis)
{
	
	if(Axis == 'X')
	{
		MD_MATH_MATRIX xr = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, MD_Math_Cos(Angle),(-1) * MD_Math_Sin(Angle), 0.0f,
			0.0f, MD_Math_Sin(Angle),MD_Math_Cos(Angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return xr;
	}
	else if('Y'){
		MD_MATH_MATRIX yr = {
			MD_Math_Cos(Angle), 0.0f, MD_Math_Sin(Angle), 0.0f,
			0.0f , 1.0f , 0.0f, 0.0f ,
			(-1) * MD_Math_Sin(Angle), 0.0f, MD_Math_Cos(Angle), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return yr;
	}
	else if('Z'){
		MD_MATH_MATRIX zr = {
			MD_Math_Cos(Angle), (-1) * MD_Math_Sin(Angle) , 0.0f, 0.0f,
			MD_Math_Sin(Angle), MD_Math_Cos(Angle), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f ,1.0f
		};
		return zr;
	}
	else
	{
		return MD_Math_IdentityMatrix;
	}
}
	
MD_MATH_MATRIX MD_Math_OrthoMatrixRH(float l, float r, float b, float t, float n, float f)
{
	MD_MATH_MATRIX result = MD_Math_IdentityMatrix;
		
	result._11 = 2.0f/(r-l);
	result._21 = 0.0f;
	result._31 = 0.0f;
	result._41 = 0.0f;
	
	result._12 = 0.0f;
	result._22 = 2.0f/(t-b);
	result._32 = 0.0f;
	result._42 = 0.0f;
	
	result._13 = 0.0f;
	result._23 = 0.0f;
	result._33 = 2.0f/(n-f);
	result._43 = 0.0f;
	
	result._14 = -(r+l)/(r-l);
	result._24 = -(t+b)/(t-b);
	result._34 = (n+f)/(n-f);
	result._44 = 1.0f;
	
	return result;
}
	
MD_MATH_MATRIX MD_Math_PerspectiveMatrixRH(float fovy, float aspect, float zNear, float zFar)
{
	MD_MATH_MATRIX result = {0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f}; 

	float f = 1.0f / MD_Math_Tan(fovy / 2.0f);

	result._11  = f / aspect;  
	result._22  = f;           
	result._33 = (zFar + zNear) / (zNear - zFar);  
	result._34 = -1.0f;                         
	result._43 = (2.0f * zFar * zNear) / (zNear - zFar); 
	
	return MD_Math_MatrixTranspose(result);
}
	
MD_MATH_MATRIX MD_Math_ViewMatrixRH(MD_MATH_VECTOR3 eye, MD_MATH_VECTOR3 target, MD_MATH_VECTOR3 up)
{
	MD_MATH_MATRIX view;

    MD_MATH_VECTOR3 cameraPos = MD_Math_Vector3Multiplication(
        eye, -1.0f
    );

    MD_MATH_VECTOR3 cameraDirection = MD_Math_Vector3Normalized(
        MD_Math_Vector3Addition(
            eye, MD_Math_Vector3Multiplication(target, -1.0f)
        )
    );
    MD_MATH_VECTOR3 cameraRight = MD_Math_Vector3Normalized(
        MD_Math_VectorCross(
            up, cameraDirection
        )
    );
    MD_MATH_VECTOR3 cameraUp = MD_Math_VectorCross(
        cameraDirection, cameraRight
    );

    view._11 = cameraRight.x;
    view._21 = cameraUp.x;
    view._31 = cameraDirection.x;
    view._41 = 0.0f;

    view._12 = cameraRight.y;
    view._22 = cameraUp.y;
    view._32 = cameraDirection.y;
    view._42 = 0.0f;

    view._13 = cameraRight.z;
    view._23 = cameraUp.z;
    view._33 = cameraDirection.z;
    view._43 = 0.0f;

    view._14 = cameraPos.x * cameraRight.x + cameraPos.y * cameraRight.y + cameraPos.z * cameraRight.z;
    view._24 = cameraPos.x * cameraUp.x + cameraPos.y * cameraUp.y + cameraPos.z * cameraUp.z;
    view._34 = cameraPos.x * cameraDirection.x + cameraPos.y * cameraDirection.y + cameraPos.z * cameraDirection.z;
    view._44 = 1.0f;

	return view;
}

MD_MATH_MATRIX MD_Math_OrthoMatrixLH(float l, float r, float b, float t, float n, float f)
{
    MD_MATH_MATRIX result = MD_Math_IdentityMatrix;

    result._11 = 2.0f / (r - l);
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;

    result._21 = 0.0f;
    result._22 = 2.0f / (t - b);
    result._23 = 0.0f;
    result._24 = 0.0f;

    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = 1.0f / (f - n);      
    result._34 = 0.0f;

    result._41 = -(r + l) / (r - l);  
    result._42 = -(t + b) / (t - b);  
    result._43 = -n / (f - n);       
    result._44 = 1.0f;

    return result;
}

MD_MATH_MATRIX MD_Math_PerspectiveMatrixLH(float fovy, float aspect, float zNear, float zFar)
{
    MD_MATH_MATRIX result = { 0 };
    float f = 1.0f / MD_Math_Tan(fovy / 2.0f);

    result._11 = f / aspect;
    result._22 = f;
    result._33 = zFar / (zFar - zNear);      
    result._34 = 1.0f;                      
    result._43 = -zNear * zFar / (zFar - zNear); 

    return MD_Math_MatrixTranspose(result); 
}

MD_MATH_MATRIX MD_Math_ViewMatrixLH(MD_MATH_VECTOR3 eye, MD_MATH_VECTOR3 target, MD_MATH_VECTOR3 up)
{
    MD_MATH_MATRIX view;

    MD_MATH_VECTOR3 cameraPos = MD_Math_Vector3Multiplication(
        eye, -1.0f
    );

    MD_MATH_VECTOR3 cameraDirection = MD_Math_Vector3Normalized(
        MD_Math_Vector3Addition(
            eye, MD_Math_Vector3Multiplication(target, -1.0f)
        )
    );
    MD_MATH_VECTOR3 cameraRight = MD_Math_Vector3Normalized(
        MD_Math_VectorCross(
            up, cameraDirection
        )
    );
    MD_MATH_VECTOR3 cameraUp = MD_Math_VectorCross(
        cameraDirection, cameraRight
    );

    view._11 = cameraRight.x;
    view._21 = cameraUp.x;
    view._31 = cameraDirection.x;
    view._41 = 0.0f;

    view._12 = cameraRight.y;
    view._22 = cameraUp.y;
    view._32 = cameraDirection.y;
    view._42 = 0.0f;

    view._13 = cameraRight.z;
    view._23 = cameraUp.z;
    view._33 = cameraDirection.z;
    view._43 = 0.0f;

    view._14 = -1.0f * (cameraPos.x * cameraRight.x + cameraPos.y * cameraRight.y + cameraPos.z * cameraRight.z);
    view._24 = -1.0f * (cameraPos.x * cameraUp.x + cameraPos.y * cameraUp.y + cameraPos.z * cameraUp.z);
    view._34 = -1.0f * (cameraPos.x * cameraDirection.x + cameraPos.y * cameraDirection.y + cameraPos.z * cameraDirection.z);
    view._44 = 1.0f;

	return view;
}
	
MD_MATH_MATRIX MD_Math_ReflectMatrix(MD_MATH_PLANE p)
{
	MD_MATH_MATRIX reflect = MD_Math_IdentityMatrix;
	
	MD_MATH_PLANE plane = MD_Math_PlaneNormalize(p);
	
	const float a = plane.a;
	const float b = plane.b;
	const float c = plane.c;
	const float d = plane.d;
	
	reflect._11 = 1 - 2*a*a;
	reflect._12 = -2*a*b;
	reflect._13 = -2*a*c;
	reflect._14 = -2*a*d;
	
	reflect._21 = -2*a*b;
	reflect._22 = 1 - 2*b*b;
	reflect._23 = -2*b*c;
	reflect._24 = -2*b*d;
	
	reflect._31 = -2*a*c;
	reflect._32 = -2*b*c;
	reflect._33 = 1 - 2*c*c;
	reflect._34 = -2*c*d;
	
	return reflect;
}

	//If OpenGL Need Transpose
MD_MATH_MATRIX MD_Math_Shadow(MD_MATH_VECTOR4 Light, MD_MATH_PLANE p)
{
	MD_MATH_MATRIX shadow = {0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f,
	                         0.0f,0.0f,0.0f,0.0f};
	
	MD_MATH_PLANE plane = MD_Math_PlaneNormalize(p);
	const float a = plane.a;
	const float b = plane.b;
	const float c = plane.c;
	const float d = plane.d;
	
	const float lx = Light.x;
	const float ly = Light.y;
	const float lz = Light.z;
	const float lw = Light.w;
	
	const float dot = a * lx + b * ly + c * lz + d * lw;
	
	shadow._11 = dot - a * lx;
	shadow._21 = - b * lx;
	shadow._31 = - c * lx;
	shadow._41 = - d * lx;
	
	shadow._12 = - a * ly;
	shadow._22 = dot - b * ly;
	shadow._32 = - c * ly;
	shadow._42 = - d * ly;
	
	shadow._13 = - a * lz;
	shadow._23 = - b * lz;
	shadow._33 = dot - c * lz;
	shadow._43 = - d * lz;
	
	shadow._14 = - a * lw;
	shadow._24 = - b * lw;
	shadow._34 = - c * lw;
	shadow._44 = dot - d * lw;
	
	return shadow;
}
	
#ifdef __cplusplus
}
#endif
