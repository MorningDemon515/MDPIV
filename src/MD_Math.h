#ifndef MD_MATH_H
#define MD_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const float MD_MATH_PI;
//Take 100 decimal places for pi

#define MD_MATH_2PI 6.283185307179586f 
#define MD_MATH_PI_2   1.57079632679489661923f
#define MD_MATH_PI_4 0.78539816339744830962f
#define MD_MATH_3_2_PI 4.71238898038468985769f

extern const float MD_MATH_E;

extern const float MD_MATH_PIReciprocal;

extern const float MD_MATH_THIRD;

//The value of root number 2 must be 100 digits
extern const float MD_MATH_SQRT2;
//The value of root number 3 must be 100 digits
extern const float MD_MATH_SQRT3;

extern const float MD_MATH_OneHundredAndEighty;

extern const float MD_MATH_1Degree;
extern const float MD_MATH_1Radian;

extern const float MD_MATH_EPSILON;

extern const float MD_MATH_LN2;

//Special trigonometric values
#define MD_MATH_SIN0  0.0f, 
#define MD_MATH_SIN30  0.5f,
#define MD_MATH_SIN45  MD_MATH_SQRT2 * 0.5f,
#define MD_MATH_SIN60  MD_MATH_SQRT3 * 0.5f,
#define MD_MATH_SIN90  1.0f,
#define MD_MATH_SIN120  MD_MATH_SQRT3 * 0.5f,
#define MD_MATH_SIN150  0.5f,
#define MD_MATH_SIN180  0.0f,
#define MD_MATH_SIN270  -1.0f;

#define MD_MATH_COS0  1.0f, 
#define MD_MATH_COS30  MD_MATH_SQRT3 * 0.5f,
#define MD_MATH_COS45  MD_MATH_SQRT2 * 0.5f,
#define MD_MATH_COS60  0.5f,
#define MD_MATH_COS90  0.0f,
#define MD_MATH_COS120  -1.0f * 0.5f,
#define MD_MATH_COS150  -1.0f * MD_MATH_SQRT3 *0.5f,
#define MD_MATH_COS180  -1.0f,
#define MD_MATH_COS270  0.0f;   

#define MD_MATH_TAN0  0.0f, 
#define MD_MATH_TAN30 MD_MATH_SQRT3 * MD_MATH_THIRD,
#define MD_MATH_TAN45  1.0f,
#define MD_MATH_TAN60  MD_MATH_SQRT3,
#define MD_MATH_TAN120  -1.0f * MD_MATH_SQRT3,
#define MD_MATH_TAN150  -1.0f * MD_MATH_SQRT3 * MD_MATH_THIRD,
#define MD_MATH_TAN180  0.0f;                      

float MD_Math_Abs(float number);

float MD_Math_Mod(float number1, float number2);

float MD_Math_Pow(float x, int y);

float MD_Math_Max(float number1, float number2);

float MD_Math_Min(float number1, float number2);

float MD_Math_Sqrt(float number);

float MD_Math_Cbrt(float number);

float MD_Math_Rsqrt(float number);

float MD_Math_Hypot(float x,float y);

float MD_Math_Factorial(int number);

float MD_Math_Sin(float x);

float MD_Math_Cos(float x);

float MD_Math_Tan(float x );

float MD_Math_Cot(float x);

float MD_Math_ArcSin(float x);

float MD_Math_ArcCos(float x);

float MD_Math_ArcTan(float x);

float MD_Math_Sec(float x);

float MD_Math_Csc(float x);

bool MD_Math_Equal(float a,float b, float epsilon);

float MD_Math_EtoXPower(float x);

float MD_Math_lnx(float x);

float MD_Math_lgx(float x);

float MD_Math_log2(float x);

float MD_Math_AngularToRadian(float angle);

float MD_Math_RadianToangle(float Radian);

float MD_Math_ComputeRectanglePerimeter(float Length, float Width);
float MD_Math_ComputeSquarePerimeter(float Length);

float MD_Math_ComputeRectangleArea(float Length, float Width);

float MD_Math_ComputeSquareArea(float Length);

float MD_Math_ComputeTriangleArea(float Bottom, float Height);

float MD_Math_ComputeParallelogramArea(float Bottom, float Height);

float MD_Math_ComputeTrapezoidArea(float Top, float Bottom,float Height);

float MD_Math_ComputeCirclePerimeter(float radius);

float MD_Math_ComputeCircleArea(float radius);

float MD_Math_ComputeCuboidVolume(float Length, float Width, float Height);

float MD_Math_ComputeCubeVolume(float Length);

float MD_Math_ComputeCylinderSideArea(float radius, float height);

float MD_Math_ComputeConeSideArea(float radius, float busbar);

float MD_Math_ComputeRoundTableSideArea(float radius1,float radius2,float busbar);

float MD_Math_ComputeCylinderSurfaceArea(float radius, float height);
float MD_Math_ComputeConeSurfaceArea(float radius, float busbar);
float MD_Math_ComputeRoundTableSurfaceArea(float radius1,float radius2, float busbar);

float MD_Math_ComputeBallSurfaceArea(float radius);

float MD_Math_ComputeCylinderVolume(float radius, float height);
float MD_Math_ComputeConeVolume(float radius, float height);

float MD_Math_ComputeRoundTableVolume(float radius1, float radius2, float height);
float MD_Math_ComputeBallVolume(float radius);
typedef struct MD_MATH_VECTOR2
{
    float x,y;
} MD_MATH_VECTOR2;

typedef struct MD_MATH_VECTOR3
{
    float x,y,z;
} MD_MATH_VECTOR3;

typedef struct MD_MATH_VECTO4
{
    float x,y,z,w;
} MD_MATH_VECTOR4;

typedef struct MD_MATH_MATRIX
{
    float _11, _12, _13, _14,
          _21, _22, _23, _24,
          _31, _32, _33, _34,
          _41, _42, _43, _44;
} MD_MATH_MATRIX;

typedef struct MD_MATH_MATRIX2x2
{
    float _11, _12, 
          _21, _22;
} MD_MATH_MATRIX2x2;

typedef struct MD_MATH_MATRIX3x3
{
    float _11, _12, _13,
          _21, _22, _23,
          _31, _32, _33;
} MD_MATH_MATRIX3x3;
 
extern const MD_MATH_VECTOR2 BasisVector2I;
extern const MD_MATH_VECTOR2 BasisVector2J;

extern const MD_MATH_VECTOR3 BasisVector3I;
extern const MD_MATH_VECTOR3 BasisVector3J;

extern const MD_MATH_VECTOR4 BasisVector4I;
extern const MD_MATH_VECTOR4 BasisVector4J;

extern const MD_MATH_VECTOR3 BasisVector3K;
extern const MD_MATH_VECTOR4 BasisVector4K;

extern const MD_MATH_VECTOR4 BasisVector4W;

extern const MD_MATH_MATRIX MD_Math_IdentityMatrix;


// About Vector--------------------------------------------------------------------------------
bool MD_Math_Vector2Equal(MD_MATH_VECTOR2 v1,MD_MATH_VECTOR2 v2);

bool MD_Math_Vector3Equal(MD_MATH_VECTOR3 v1,MD_MATH_VECTOR3 v2);

bool MD_Math_Vector4Equal(MD_MATH_VECTOR4 v1,MD_MATH_VECTOR4 v2);

MD_MATH_VECTOR2 MD_Math_Vector2Addition(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2);
MD_MATH_VECTOR3 MD_Math_Vector3Addition(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2);

MD_MATH_VECTOR4 MD_Math_Vector4Addition(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2);

MD_MATH_VECTOR2 MD_Math_Vector2Multiplication(MD_MATH_VECTOR2 v,float x);

MD_MATH_VECTOR3 MD_Math_Vector3Multiplication(MD_MATH_VECTOR3 v,float x);

MD_MATH_VECTOR4 MD_Math_Vector4Multiplication(MD_MATH_VECTOR4 v,float x);
float MD_Math_ComputeVector2Length(MD_MATH_VECTOR2 v);

float MD_Math_ComputeVector3Length(MD_MATH_VECTOR3 v);

float MD_Math_ComputeVector4Length(MD_MATH_VECTOR4 v);

MD_MATH_VECTOR2 MD_Math_Vector2Normalized(MD_MATH_VECTOR2 v);

MD_MATH_VECTOR3 MD_Math_Vector3Normalized(MD_MATH_VECTOR3 v);

MD_MATH_VECTOR4 MD_Math_Vector4Normalized(MD_MATH_VECTOR4 v);

float MD_Math_Vector2Dot(MD_MATH_VECTOR2 v1,MD_MATH_VECTOR2 v2);

float MD_Math_Vector3Dot(MD_MATH_VECTOR3 v1,MD_MATH_VECTOR3 v2);

float MD_Math_Vector4Dot(MD_MATH_VECTOR4 v1,MD_MATH_VECTOR4 v2);

MD_MATH_VECTOR3 MD_Math_VectorCross(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2);

float MD_Math_ProjectionOfVector2(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2);

float MD_Math_ProjectionOfVector3(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2);

float MD_Math_ProjectionOfVector4(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2);

MD_MATH_VECTOR2 MD_Math_ProjectionVector2(MD_MATH_VECTOR2 v1, MD_MATH_VECTOR2 v2);

MD_MATH_VECTOR3 MD_Math_ProjectionVector3(MD_MATH_VECTOR3 v1, MD_MATH_VECTOR3 v2);
MD_MATH_VECTOR4 MD_Math_ProjectionVector4(MD_MATH_VECTOR4 v1, MD_MATH_VECTOR4 v2);
//---------------------------------------------------------------------------------------------------------
//About Matrix --------------------------------------------------------------------------------------------

MD_MATH_VECTOR4 MD_Math_VectorMulMatrix(MD_MATH_MATRIX m, MD_MATH_VECTOR4 v);

MD_MATH_MATRIX MD_Math_MatrixMulMatrix(MD_MATH_MATRIX m1,MD_MATH_MATRIX m2);


MD_MATH_MATRIX MD_Math_MatrixAddMatrix(MD_MATH_MATRIX m1,MD_MATH_MATRIX m2);

MD_MATH_MATRIX MD_Math_MatrixMultiplication(MD_MATH_MATRIX m,float x);

MD_MATH_MATRIX MD_Math_MatrixTranspose(MD_MATH_MATRIX m);

float MD_Math_DetMatrix2x2(MD_MATH_MATRIX2x2 m);

float MD_Math_DetMatrix3x3(MD_MATH_MATRIX3x3 m);
float MD_Math_DetMatrix4x4(MD_MATH_MATRIX m);

MD_MATH_MATRIX MD_Math_AdjointMatrix(MD_MATH_MATRIX m);

MD_MATH_MATRIX MD_Math_InvMatrix(MD_MATH_MATRIX m);

bool MD_Math_MatrixEqual(MD_MATH_MATRIX m1 , MD_MATH_MATRIX m2);

void MD_Math_MatrixToValue(MD_MATH_MATRIX m, float* out);

//Plane-------------------------------------------------------------------------------

typedef struct MD_MATH_PLANE  
{
    float a,b,c,d;
} MD_MATH_PLANE;

MD_MATH_PLANE MD_Math_PlaneFromPointAndNormal(MD_MATH_VECTOR3 Point,MD_MATH_VECTOR3 Normal);

MD_MATH_PLANE MD_Math_PlaneFromPoints(MD_MATH_VECTOR3 p0,
                                      MD_MATH_VECTOR3 p1,
                                      MD_MATH_VECTOR3 p2);
									  
float MD_Math_PlaneDot(MD_MATH_PLANE p, MD_MATH_VECTOR3 v);
MD_MATH_PLANE MD_Math_PlaneNormalize(MD_MATH_PLANE p);
MD_MATH_PLANE MD_Math_PlaneTransform(MD_MATH_PLANE p, MD_MATH_MATRIX m);

//Ray-----------------------------------------------------------------

typedef struct MD_MATH_RAY
{
    MD_MATH_VECTOR3 p0,u;
} MD_MATH_RAY;


MD_MATH_VECTOR3 MD_Math_CreateRay(MD_MATH_RAY ray,float t);
MD_MATH_VECTOR3 MD_Math_Intersection(MD_MATH_RAY ray, MD_MATH_PLANE p);
	
MD_MATH_MATRIX MD_Math_ScaleMatrix(float x, float y, float z);
MD_MATH_MATRIX MD_Math_TranslationMatrix(float x, float y, float z);
MD_MATH_MATRIX MD_Math_RotationMatrix(float Angle, char Axis);
	
MD_MATH_MATRIX MD_Math_PerspectiveMatrixRH(float fovy, float aspect, float zNear, float zFar);	
MD_MATH_MATRIX MD_Math_OrthoMatrixRH(float l, float r, float b, float t, float n, float f);	
MD_MATH_MATRIX MD_Math_ViewMatrixRH(MD_MATH_VECTOR3 eye, MD_MATH_VECTOR3 target, MD_MATH_VECTOR3 up);	
	
MD_MATH_MATRIX MD_Math_PerspectiveMatrixLH(float fovy, float aspect, float zNear, float zFar);
MD_MATH_MATRIX MD_Math_OrthoMatrixLH(float l, float r, float b, float t, float n, float f);
MD_MATH_MATRIX MD_Math_ViewMatrixLH(MD_MATH_VECTOR3 eye, MD_MATH_VECTOR3 target, MD_MATH_VECTOR3 up);

MD_MATH_MATRIX MD_Math_ReflectMatrix(MD_MATH_PLANE p);	
MD_MATH_MATRIX MD_Math_Shadow(MD_MATH_VECTOR4 Light, MD_MATH_PLANE p);	

#ifdef __cplusplus
}
#endif

#endif
