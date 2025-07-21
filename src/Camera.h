#ifndef CAMERA_H
#define CAMERA_H

#include "MD_Math.h"
#include "Main.h"

class Camera
{
private:
   MD_MATH_VECTOR3 cameraPos = {0.0f, 0.0f,  3.0f};
   MD_MATH_VECTOR3 cameraFront = {0.0f, 0.0f, -1.0f};
   MD_MATH_VECTOR3 cameraUp = {0.0f, 1.0f,  0.0f};

   //float lastX = WINDOW_WIDTH / 2 , lastY = WINDOW_HEIGHT / 2;

   //POINT Cp = {0, 0};

   float pitch = 0.0f , yaw = -90.0f;

   float cameraSpeed = 0.05f;
   float sensitivity = 0.8f;
public:
    Camera();
    ~Camera();

    void Move();
    void Look();
    MD_MATH_MATRIX Matrix();

};




#endif