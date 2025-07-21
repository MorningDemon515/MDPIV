#include "Camera.h"

#include "SimpleInput.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Move()
{
    if(Input_IsKeyDown('W'))
        cameraPos = MD_Math_Vector3Addition(cameraPos , (MD_Math_Vector3Multiplication(cameraFront, cameraSpeed)));

    if(Input_IsKeyDown('S'))
        cameraPos = MD_Math_Vector3Addition(cameraPos , (MD_Math_Vector3Multiplication(cameraFront, (-1.0f) *cameraSpeed)));

    if(Input_IsKeyDown('A'))
        cameraPos = MD_Math_Vector3Addition(cameraPos,
            MD_Math_Vector3Multiplication(MD_Math_Vector3Normalized(MD_Math_VectorCross(cameraFront, cameraUp)),(-1.0f) * cameraSpeed));

    if(Input_IsKeyDown('D'))
        cameraPos = MD_Math_Vector3Addition(cameraPos,
            MD_Math_Vector3Multiplication(MD_Math_Vector3Normalized(MD_Math_VectorCross(cameraFront, cameraUp)),cameraSpeed)); 
}

void Camera::Look()
{
    /*
    Cp = Input_GetMousePosition();

    float xoffset = Cp.x - lastX;
    float yoffset = lastY - Cp.y; 
    lastX = Cp.x;
    lastY = Cp.y;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;
    */

    if(Input_IsKeyDown(VK_LEFT))
        yaw -= sensitivity;
    if(Input_IsKeyDown(VK_RIGHT))
        yaw += sensitivity;
    if(Input_IsKeyDown(VK_UP))
        pitch += sensitivity;
    if(Input_IsKeyDown(VK_DOWN))
        pitch -= sensitivity;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    cameraFront.x = MD_Math_Cos(MD_Math_AngularToRadian(yaw)) * MD_Math_Cos(MD_Math_AngularToRadian(pitch)); 
    cameraFront.y = MD_Math_Sin(MD_Math_AngularToRadian(pitch));
    cameraFront.z = MD_Math_Sin(MD_Math_AngularToRadian(yaw)) * MD_Math_Cos(MD_Math_AngularToRadian(pitch));
    cameraFront = MD_Math_Vector3Normalized(cameraFront);
}

MD_MATH_MATRIX Camera::Matrix()
{
    MD_MATH_MATRIX view = MD_Math_ViewMatrixRH(
         cameraPos,
         MD_Math_Vector3Addition(cameraPos, cameraFront),
         cameraUp
    );   

    return view;
}