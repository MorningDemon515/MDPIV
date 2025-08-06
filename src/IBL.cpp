#include "IBL.h"

#include <glad/glad.h>
#include "Shader.h"

#include "stb_image.h"

using namespace MD_Math;

IBL::IBL(const char* hdr)
{
    Cube skybox = Cube();

    Shader skyboxshader("resources/glsl/ToCubemap_vs.txt","resources/glsl/ToCubemap_fs.txt");
    skyboxshader.Link();
    
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO); 

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(hdr, &width, &height, &nrComponents, 0);
   
    glGenTextures(1, &HDRtexture);
    glBindTexture(GL_TEXTURE_2D, HDRtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glGenTextures(1, &CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    MATRIX captureProjection = PerspectiveMatrixRH(AngularToRadian(90.0f), 1.0f, 0.1f, 10.0f);
    MATRIX captureViews[] = 
    {
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 1.0f,  0.0f,  0.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(-1.0f,  0.0f,  0.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  1.0f,  0.0f), VECTOR3(0.0f,  0.0f,  1.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f, -1.0f,  0.0f), VECTOR3(0.0f,  0.0f, -1.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  0.0f,  1.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  0.0f, -1.0f), VECTOR3(0.0f, -1.0f,  0.0f))
    };

    // convert HDR equirectangular environment map to cubemap equivalent
    skyboxshader.Use();
    skyboxshader.SetInt("equirectangularMap", 0);
    skyboxshader.SetMatrix("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, HDRtexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        skyboxshader.SetMatrix("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, CubemapTexture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.Draw(skyboxshader); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

IBL::~IBL()
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteTextures(1, &HDRtexture);
    glDeleteTextures(1, &CubemapTexture);
}

void IBL::Set(Shader& FinalSkyBoxShader)
{
    FinalSkyBoxShader.Link();
    FinalSkyBoxShader.Use();
    FinalSkyBoxShader.SetInt("equirectangularMap", 0);
}

void IBL::Draw(Cube& FinalSkyBox, Shader& FinalSkyBoxShader)
{    
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture);
        FinalSkyBox.Draw(FinalSkyBoxShader);
}