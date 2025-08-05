#ifndef MESH_H
#define MESH_H

#include "MD_Math.h"
#include <iostream>
#include <vector>

#include "Shader.h"

struct Vertex
{
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Normals;
    MD_Math::VECTOR2 TexCoords;
    MD_Math::VECTOR3 Tangent;   
    MD_Math::VECTOR3 Bitangent; 
};

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;

    void ComputeTangents(std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices);
public:
    Mesh(){};
    Mesh(MD_Math::VECTOR3 pos[], MD_Math::VECTOR2 texc[], std::vector<unsigned int> indices);
    ~Mesh();

    void Draw(Shader& shader);
};

class Cube
{
private:
    unsigned int VAO, VBO;
public:
    Cube();
    ~Cube();

    void Draw(Shader& shader);
};

#endif