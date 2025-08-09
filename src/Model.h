#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <map>
#include <vector>

class Model
{
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    void LoadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    
    Assimp::Importer import;
    const aiScene *scene;
    aiMesh *mesh;
    aiFace face;
    aiMaterial* material;

    std::string directory;
public:
    Model(){};
    Model(std::string path)
    {
        LoadModel(path);
    }
    
    void Draw(Shader& shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void Free()
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Free();

        import.FreeScene();    

    }

    unsigned int texCount;
};

#endif