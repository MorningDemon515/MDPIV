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
        cameraPos += cameraFront * cameraSpeed;

    if(Input_IsKeyDown('S'))
        cameraPos -= cameraFront * cameraSpeed;

    if(Input_IsKeyDown('A'))
        cameraPos -= MD_Math::Vector3Normalized(MD_Math::VectorCross(cameraFront, cameraUp)) * cameraSpeed;

    if(Input_IsKeyDown('D'))
        cameraPos += MD_Math::Vector3Normalized(MD_Math::VectorCross(cameraFront, cameraUp)) * cameraSpeed; 
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

    cameraFront.x = MD_Math::Cos(MD_Math::AngularToRadian(yaw)) * MD_Math::Cos(MD_Math::AngularToRadian(pitch)); 
    cameraFront.y = MD_Math::Sin(MD_Math::AngularToRadian(pitch));
    cameraFront.z = MD_Math::Sin(MD_Math::AngularToRadian(yaw)) * MD_Math::Cos(MD_Math::AngularToRadian(pitch));
    cameraFront = MD_Math::Vector3Normalized(cameraFront);
}

MD_Math::MATRIX Camera::Matrix()
{
    MD_Math::MATRIX view = MD_Math::ViewMatrixRH(
         cameraPos,
         cameraPos + cameraFront,
         cameraUp
    );   

    return view;
}

MD_Math::VECTOR3 Camera::Pos()
{
    return cameraPos;
}

MD_Math::VECTOR3 Camera::Front()
{
    return cameraFront;
}