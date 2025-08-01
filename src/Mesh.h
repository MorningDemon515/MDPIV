#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

//#define MAX_BONE_INFLUENCE 4

struct Vertex {
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Normal;
    MD_Math::VECTOR2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    
    Mesh(){};
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif 