#ifndef CAMERA_H
#define CAMERA_H

#include "MD_Math.h"
#include "Input.h"

class Camera
{
private:
    MD_Math::VECTOR3 cameraPos = {0.0f, 0.0f, 3.0f};
    MD_Math::VECTOR3 cameraFront = {0.0f, 0.0f, -1.0f};
    MD_Math::VECTOR3 cameraUp = {0.0f, 1.0f, 0.0f};

    float pitch = 0.0f, yaw = -90.0f;
public:
    Camera();
    ~Camera();

    void Move(float cameraSpeed, float mouseSpeed)
    {
        if(Input_IsKeyDown(GLFW_KEY_W))
            cameraPos += cameraFront * cameraSpeed;

        if(Input_IsKeyDown(GLFW_KEY_S))
            cameraPos -= cameraFront * cameraSpeed;

        if(Input_IsKeyDown(GLFW_KEY_A))
            cameraPos -= MD_Math::Vector3Normalized(MD_Math::VectorCross(cameraFront, cameraUp)) * cameraSpeed;

        if(Input_IsKeyDown(GLFW_KEY_D))
            cameraPos += MD_Math::Vector3Normalized(MD_Math::VectorCross(cameraFront, cameraUp)) * cameraSpeed;
        
        if(Input_IsKeyDown(GLFW_KEY_UP))
            pitch += mouseSpeed;

        if(Input_IsKeyDown(GLFW_KEY_DOWN))
            pitch -= mouseSpeed;    

        if(Input_IsKeyDown(GLFW_KEY_LEFT))
            yaw -= mouseSpeed;

        if(Input_IsKeyDown(GLFW_KEY_RIGHT))
            yaw += mouseSpeed;        

        cameraFront.x = MD_Math::Cos(MD_Math::AngularToRadian(pitch)) * MD_Math::Cos(MD_Math::AngularToRadian(yaw));
        cameraFront.y = MD_Math::Sin(MD_Math::AngularToRadian(pitch));
        cameraFront.z = MD_Math::Cos(MD_Math::AngularToRadian(pitch)) * MD_Math::Sin(MD_Math::AngularToRadian(yaw));
        cameraFront = MD_Math::Vector3Normalized(cameraFront);    
    }

    MD_Math::MATRIX Matrix()
    {
        return MD_Math::ViewMatrixRH(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );
    }
};

Camera::Camera()
{
}

Camera::~Camera()
{
}


#endif